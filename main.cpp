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

TcpServer srv;
Serial serial;

int main(int argc, char *argv[]) {
    if (!ToolKits::is_file_exists(UART0_PATH)) {
        return 0;//非主系统,娱乐系统不存在串口模块
    }
    int port = 1883;

    hlog_set_level(LOG_LEVEL_DEBUG);

    int listenfd = srv.createsocket(port);
    if (listenfd < 0) {
        return -20;
    }
    printf("Build Time: %s %s\n", __TIME__, __DATE__);
    printf("server listen on port %d, listenfd=%d ...\n", port, listenfd);

    //环境初始化
    ToolKits::EnvInit();
    //串口初始化
    serial.UART_init();

    Verification verification(serial, srv);
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
    m_srv.withTLS(&ssl_opt);
#endif

    srv.start();

    if (ToolKits::is_file_exists("/data/local/demo.lock")) {
        verification.demoThread();
    }
    verification.releaseThread();

    while (true) {
        sleep(60);
    }
    return 0;
}
