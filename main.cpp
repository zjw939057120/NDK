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
#include "hv/TcpClient.h"
#include "hv/htime.h"
#include "Serial.h"
#include "Verification.h"
#include "VerificationReceive.h"
#include "ToolKits.h"
#include "ClientMessageCallback.h"

using namespace hv;

#define TEST_TLS 0

int UART_TcpServer_Instance(TcpServer &srv, int port, Verification &verification,
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

int TCP_Client_Instance(int remote_port, const char *remote_host, const std::function<void(Buffer *)> &onMessage) {

    TcpClient cli;
    int connfd = cli.createsocket(remote_port, remote_host);
    if (connfd < 0) {
        return -20;
    }
    printf("client connect to port %d, connfd=%d ...\n", remote_port, connfd);
    cli.onConnection = [&cli](const SocketChannelPtr &channel) {
        std::string peeraddr = channel->peeraddr();
        if (channel->isConnected()) {
            printf("connected to %s! connfd=%d\n", peeraddr.c_str(), channel->fd());
        } else {
            printf("disconnected to %s! connfd=%d\n", peeraddr.c_str(), channel->fd());
        }
        if (cli.isReconnect()) {
            printf("reconnect cnt=%d, delay=%d\n", cli.reconn_setting->cur_retry_cnt, cli.reconn_setting->cur_delay);
        }
    };
    
    cli.onMessage = [onMessage](const SocketChannelPtr &channel, Buffer *buf) {
        onMessage(buf);
    };

#if TEST_RECONNECT
    // reconnect: 1,2,4,8,10,10,10...
    reconn_setting_t reconn;
    reconn_setting_init(&reconn);
    reconn.min_delay = 1000;
    reconn.max_delay = 10000;
    reconn.delay_policy = 2;
    cli.setReconnect(&reconn);
#endif

#if TEST_TLS
    cli.withTLS();
#endif

    cli.start();

    std::string str;
    while (std::getline(std::cin, str)) {
        if (str == "close") {
            cli.closesocket();
        } else if (str == "start") {
            cli.start();
        } else if (str == "stop") {
            cli.stop();
            break;
        } else {
            if (!cli.isConnected()) break;
            cli.send(str);
        }
    }

    return 0;
}

int main(int argc, char *argv[]) {
    //记录主板唯一ID
    ToolKits::getSerialNumber();

    //环境初始化开始
    if (!ToolKits::isFileExists(TCP_SERVER_LOCK)) {
        if (ToolKits::isFileExists(UART1_PATH)) {
            //存在串口模块判定为中控系统
            printf("the %s found\r\n", UART1_PATH);
            //禁用娱乐屏app
            ToolKits::disablePackage(PACKAGE_COM_HBTENGLV_BOAT_HOME);
        } else {
            //禁用中控屏app
            ToolKits::disablePackage(PACKAGE_COM_HBTENGLV_BOAT);
        }
        FILE *fp = fopen(TCP_SERVER_LOCK, "w");
        fclose(fp);
    }
    //环境初始化结束

    Serial serial;
    TcpServer svr_sys;
    TcpServer srv_0;
    TcpServer srv_1;
    TcpServer srv_2;
    TcpServer srv_3;


    //中控屏服务端应用
    if (ToolKits::isDeviceExist(EC20_PATH)) {
    //串口初始化
    serial.UART_init();

    //实例化服务器消息处理
    Verification verification(serial, svr_sys, srv_0, srv_1, srv_2, srv_3);

    //实例化串口消息处理
    VerificationReceive verificationReceive(serial, srv_0, srv_1, srv_2, srv_3);

        //系统服务
        UART_TcpServer_Instance(svr_sys, SYS_PORT, verification,
                                std::bind(&Verification::svr_sys_onMessageCallback, &verification,
                                          std::placeholders::_1));

    //串口0透传服务
    UART_TcpServer_Instance(srv_0, UART0_PORT, verification,
                   std::bind(&Verification::svr0_onMessageCallback, &verification, std::placeholders::_1));

    //串口1透传服务
    UART_TcpServer_Instance(srv_1, UART1_PORT, verification,
                   std::bind(&Verification::svr1_onMessageCallback, &verification, std::placeholders::_1));

    //串口2透传服务
    UART_TcpServer_Instance(srv_2, UART2_PORT, verification,
                   std::bind(&Verification::svr2_onMessageCallback, &verification, std::placeholders::_1));

    //串口3透传服务
    UART_TcpServer_Instance(srv_3, UART3_PORT, verification,
                   std::bind(&Verification::svr3_onMessageCallback, &verification, std::placeholders::_1));

    if (ToolKits::isFileExists("/data/local/demo.lock")) {
        verification.svr_demoThread();
    }

    //串口消息处理
    verificationReceive.UART_receiveThread();
    } else {
        //娱乐屏客户端应用
        sleep(1);
        std::string ip = ToolKits::getETH0Gateway();
        printf("the eth0 gateway is %s\r\n", ip.c_str());
        ClientMessageCallback clientMessageCallback;
        TCP_Client_Instance(SYS_PORT, ip.c_str(),std::bind(&ClientMessageCallback::onMessageCallback, &clientMessageCallback, std::placeholders::_1));
    }

    while (true) {
        sleep(120);
    }
    return 0;
}

