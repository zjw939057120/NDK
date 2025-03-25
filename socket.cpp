//
// Created by zjw93 on 2025/3/25.
//

#include "socket.h"
#include "client.h"

#include <iostream>
#include <unistd.h>
#include <nanomsg/nn.h>
#include <nanomsg/bus.h>
#include <nanomsg/pair.h>

#include <iostream>

#include "TcpServer.h"

#define TEST_RECONNECT  1
#define TEST_TLS        0
using namespace hv;

#include <iostream>

#include "TcpClient.h"
#include "htime.h"

#define TEST_RECONNECT  1
#define TEST_TLS        0

using namespace hv;

#define MSG_FILE_INPROC "inproc://nanomsg"
int nn_sock_server;
int nn_sock_client;

void socket_pthread(void *) {
    nn_socket_init();
    nn_client_init();

    pthread_t thread1;
    pthread_create(&thread1, NULL, reinterpret_cast<void *(*)(void *)>(jt905_tcp_client_pthread), NULL);
    pthread_t thread2;
    pthread_create(&thread2, NULL, reinterpret_cast<void *(*)(void *)>(jt905_tcp_relay_server_pthread), NULL);
    pthread_t thread3;
    pthread_create(&thread3, NULL, reinterpret_cast<void *(*)(void *)>(read_msg), NULL);//接收主TCP网络数据线程
    pthread_t thread4;
    pthread_create(&thread4, NULL, reinterpret_cast<void *(*)(void *)>(server_data_worke_thread), NULL);//主TCP网络数据解析线程
}

int jt905_tcp_relay_server_pthread() {
    int port = 1883;

    hlog_set_level(LOG_LEVEL_DEBUG);

    TcpServer srv;
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
    srv.onMessage = [](const SocketChannelPtr &channel, Buffer *buf) {
        // echo
        printf("< %.*s\n", (int) buf->size(), (char *) buf->data());
        channel->write(buf);
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

    std::string str;
    while (std::getline(std::cin, str)) {
        if (str == "close") {
            srv.closesocket();
        } else if (str == "start") {
            srv.start();
        } else if (str == "stop") {
            srv.stop();
            break;
        } else {
            srv.broadcast(str.data(), str.size());
        }
    }

    return 0;
}


TcpClient cli;

int jt905_tcp_client_pthread() {
    int remote_port = 7090;
    const char *remote_host = "1.70.236.173";

    int connfd = cli.createsocket(remote_port, remote_host);
    if (connfd < 0) {
        return -20;
    }
    printf("client connect to port %d, connfd=%d ...\n", remote_port, connfd);
    cli.onConnection = [](const SocketChannelPtr &channel) {
        std::string peeraddr = channel->peeraddr();
        if (channel->isConnected()) {
            printf("connected to %s! connfd=%d\n", peeraddr.c_str(), channel->fd());
            // send(time) every 3s
            setInterval(3000, [channel](TimerID timerID) {
                if (channel->isConnected()) {
                    if (channel->isWriteComplete()) {
                        char str[DATETIME_FMT_BUFLEN] = {0};
                        datetime_t dt = datetime_now();
                        datetime_fmt(&dt, str);
                        channel->write(str);
                    }
                } else {
                    killTimer(timerID);
                }
            });
        } else {
            printf("disconnected to %s! connfd=%d\n", peeraddr.c_str(), channel->fd());
        }
        if (cli.isReconnect()) {
            printf("reconnect cnt=%d, delay=%d\n", cli.reconn_setting->cur_retry_cnt, cli.reconn_setting->cur_delay);
        }
    };
    cli.onMessage = [](const SocketChannelPtr &channel, Buffer *buf) {
        printf("< %.*s\n", (int) buf->size(), (char *) buf->data());
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

/*    std::string str;
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
    }*/

    return 0;
}

void nn_socket_init() {
    //部署IPC进程间通讯服务端
    nn_sock_server = nn_socket(AF_SP, NN_BUS);
    if (nn_sock_server < 0) {
        perror("nn_socket");
    }
    if (nn_bind(nn_sock_server, MSG_FILE_INPROC) < 0) {  // 绑定自己的地址
        perror("nn_bind");
    }
    //连接IPC进程间通讯客户端
    nn_sock_client = nn_socket(AF_SP, NN_BUS);
    if (nn_sock_client < 0) {
        perror("nn_socket");
    }
    if (nn_connect(nn_sock_client, MSG_FILE_INPROC) < 0) {  // 连接到服务端
        perror("nn_connect");
    }
}

/*************************************************************
 * @函  数  名：
 * @函数功能：读取主服务器下行数据
 * @完成时间：
 * @输        入：无
 * @返        回：无
 * @作        者：Mr Dollar
 * @改动说明：
 *************************************************************/
void read_msg(void *arg) {
    std::string str = "ABCDEFG";
    while (1) {
        if (cli.isConnected()) {
            printf("client send %s\n", str.c_str());
            cli.send(str);
            sleep(3);
        } else {
            printf("client not isConnected\n");
            sleep(1);
            continue;
        }
    }
}


/*************************************************************
 * @函  数  名：
 * @函数功能：数据处理线程
 * @完成时间：2018-10-17
 * @输        入：无
 * @返        回：无
 * @作        者：Mr Dollar
 * @改动说明：
 *************************************************************/
void server_data_worke_thread(void *) {

}


