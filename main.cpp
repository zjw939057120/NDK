/*
 * TcpServer_test.cpp
 *
 * @build   make evpp
 * @server  bin/TcpServer_test 1234
 * @client  bin/TcpClient_test 1234
 *
 */

#include <iostream>

#include "hv/TcpServer.h"
#include "Serial.h"
#include "Verification.h"
#include "VerificationReceive.h"
#include "ToolKits.h"

using namespace hv;

#define TEST_TLS 0

void onMessageCallback(Buffer *buf) {
}

int UART_TcpServer(TcpServer &srv, int port, Verification &verification,
                   const std::function<void(Buffer *)> &onMessage) {
    hlog_set_level(LOG_LEVEL_DEBUG);

    int listenfd = srv.createsocket(port);
    if (listenfd < 0) {
        return -20;
    }
    printf("server listen on port %d, listenfd=%d ...\n", port, listenfd);
    srv.onConnection = [](const SocketChannelPtr &channel) {
        std::string peeraddr = channel->peeraddr();
        if (channel->isConnected()) {
            printf("%s connected! connfd=%d id=%d tid=%ld\n", peeraddr.c_str(), channel->fd(), channel->id(),
                   currentThreadEventLoop->tid());
        } else {
            printf("%s disconnected! connfd=%d id=%d tid=%ld\n", peeraddr.c_str(), channel->fd(), channel->id(),
                   currentThreadEventLoop->tid());
        }
    };
    srv.onMessage = [onMessage](const SocketChannelPtr &channel, Buffer *buf) {
        onMessage(buf);
    };
    srv.setThreadNum(4);
    srv.setLoadBalance(LB_LeastConnections);

#if TEST_TLS
    hssl_ctx_opt_t ssl_opt;
memset(&ssl_opt, 0, sizeof(hssl_ctx_opt_t));
ssl_opt.crt_file = "cert/server.crt";
ssl_opt.key_file = "cert/server.key";
ssl_opt.verify_peer = 0;
srv.withTLS(&ssl_opt);
#endif

    srv.start();
    return 0;
};

int main(int argc, char *argv[]) {
    //记录主板唯一ID
    ToolKits::getSerialNumber();

    //环境初始化开始
    if (!ToolKits::is_file_exists(TCP_SERVER_LOCK)) {
        if (ToolKits::is_file_exists(EC20_PATH)) {
            //存在EC20模块判定为中控系统
            printf("the %s found\r\n", EC20_PATH);
            //禁用娱乐屏app
            ToolKits::disablePackage(PACKAGE_COM_HBTENGLV_BOAT_HOME);
        } else {
            //禁用中控屏app
            ToolKits::disablePackage(PACKAGE_COM_HBTENGLV_BOAT);
        }
        fopen(TCP_SERVER_LOCK, "w");
    }
    //环境初始化结束

    Serial serial;
    TcpServer svr_sys;
    TcpServer srv_0;
    TcpServer srv_1;
    TcpServer srv_2;
    TcpServer srv_3;

    //串口初始化
    serial.UART_init();

    //实例化服务器消息处理
    Verification verification(serial, svr_sys, srv_0, srv_1, srv_2, srv_3);

    //实例化串口消息处理
    VerificationReceive verificationReceive(serial, srv_0, srv_1, srv_2, srv_3);

    //系统服务
    UART_TcpServer(svr_sys, 1870, verification,
                   std::bind(&Verification::svr_sys_onMessageCallback, &verification, std::placeholders::_1));

    //串口0透传服务
    UART_TcpServer(srv_0, 1880, verification,
                   std::bind(&Verification::svr0_onMessageCallback, &verification, std::placeholders::_1));

    //串口1透传服务
    UART_TcpServer(srv_1, 1881, verification,
                   std::bind(&Verification::svr1_onMessageCallback, &verification, std::placeholders::_1));

    //串口2透传服务
    UART_TcpServer(srv_2, 1882, verification,
                   std::bind(&Verification::svr2_onMessageCallback, &verification, std::placeholders::_1));

    //串口3透传服务
    UART_TcpServer(srv_3, 1883, verification,
                   std::bind(&Verification::svr3_onMessageCallback, &verification, std::placeholders::_1));

    if (ToolKits::is_file_exists("/data/local/demo.lock")) {
        verification.svr_demoThread();
    }

    //串口消息处理
    verificationReceive.UART_receiveThread();

    while (true) {
        sleep(120);
    }
    return 0;
}

