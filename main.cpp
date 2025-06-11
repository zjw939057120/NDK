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
    int port = 1883;

    hlog_set_level(LOG_LEVEL_DEBUG);

    int listenfd = srv.createsocket(port);
    if (listenfd < 0) {
        return -20;
    }
    printf("Build Time: %s %s\n", __TIME__, __DATE__);
    printf("server listen on port %d, listenfd=%d ...\n", port, listenfd);

    ToolKits::EnvInit();
    serial.Open();
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
        verification.Check(buf);
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
    uint8_t buffer[CAN_BUFFER_LEN]; // 读取串口数据
    while (true) {
        if (serial.Receive(buffer, CAN_BUFFER_LEN) > 0) {
            ToolKits::dump(buffer, CAN_BUFFER_LEN);
            srv.broadcast((const void *) buffer, CAN_BUFFER_LEN); // 转发串口数据到tcp客户端
        }
    }

    return 0;
}
