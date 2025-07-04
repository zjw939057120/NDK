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
#include "ToolKits.h"

using namespace hv;

#define TEST_TLS 0


int UART_TcpServer(TcpServer &srv, int port, Verification &verification) {
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
    srv.onMessage = [&verification](const SocketChannelPtr &channel, Buffer *buf) {
        verification.onMessage(buf);
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
    /*
    if (!ToolKits::is_file_exists(UART0_PATH)) {
        printf("the %s not found, exit\r\n", UART0_PATH);
        return 0;//仅主系统存在CH344/CH348串口模块
    }*/
    Serial serial;

    TcpServer UART0_srv;
    TcpServer UART1_srv;
    TcpServer UART2_srv;
    TcpServer UART3_srv;

    //环境初始化
    ToolKits::EnvInit();
    //串口初始化
    serial.UART_init();

    Verification verification(serial, UART0_srv, UART1_srv, UART2_srv, UART3_srv);

    if (ToolKits::is_file_exists(UART0_PATH))
        UART_TcpServer(UART0_srv, 1880, verification);

    if (ToolKits::is_file_exists(UART1_PATH))
        UART_TcpServer(UART1_srv, 1881, verification);

    if (ToolKits::is_file_exists(UART2_PATH))
        UART_TcpServer(UART2_srv, 1882, verification);

    if (ToolKits::is_file_exists(UART3_PATH))
        UART_TcpServer(UART3_srv, 1883, verification);

    if (ToolKits::is_file_exists("/data/local/demo.lock")) {
        verification.demoThread();
    }
    verification.releaseThread();

    while (true) {
        sleep(120);
    }
    return 0;
}

