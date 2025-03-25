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
    jt905_tcp_client_init();
    jt905_tcp_relay_init();

    nn_socket_init();
    nn_client_init();

    pthread_t thread_id_1;
    pthread_t thread_id_2;
    pthread_create(&thread_id_1, NULL, reinterpret_cast<void *(*)(void *)>(read_msg), NULL);//接收主TCP网络数据线程
    pthread_create(&thread_id_2, NULL, reinterpret_cast<void *(*)(void *)>(server_data_worke_thread),
                   NULL);//主TCP网络数据解析线程
}


int jt905_tcp_relay_init() {
    int port = 5693;

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


int jt905_tcp_client_init() {
    int remote_port = 7090;
    const char *remote_host = "1.70.236.173";

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
/*    int num, i;                            // Counter of received bytes
    unsigned char revbuf[BUFFER_SIZE]; // Receive buffer

    *//* Try to connect the server *//*
    while (1)// Check remoter command
    {
        if (access("/tmp/xstrive_upgrade.upf", F_OK) == 0 || (access("/tmp/_4gmodule_found", F_OK) != 0)) {
            sleep(10);
        }

        if (net_flag == 1)//(0:断开；1:正常)
        {
            //memset (revbuf,0,256);
            num = recv(sockfd, revbuf, BUFFER_SIZE, 0);

            switch (num) {
                case -1://当ret < 0 说明出现了异常 例如阻塞状态解除，或者读取数据时出现指针错误等。所以我们这里要主动断开和客户端的链接。

                    printf("ERROR: <TCP>Receive string error! Take the initiative to disconnect \n");

                    close(sockfd);
                    net_flag = 0;//(0:断开；1:正常)
                    break;
                case 0://num == 0 说明服务器掉线。

                    printf("ERROR: <TCP>Receive string error! Server disconnected \n");

                    close(sockfd);
                    net_flag = 0;//(0:断开；1:正常)
                    break;
                default:
                    printf("Server data length = %d\n\n", num);
                    break;
            }

            //printf ("OK: Receviced string is: %X\n", revbuf);

            //获取系统时间
            struct tm *t;
            time_t tt;
            time(&tt);
            t = localtime(&tt);
            printf("Receviced Data Timer:%4d-%02d-%02d %02d:%02d:%02d    length:%d\n\n", t->tm_year + 1900,
                   t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec, num);
            printf("Server Data:\n");
            if (num > 0) {
                nn_send(nn_sock_client, &revbuf, num, 0);
                revbuf[num] = '\0';
                for (i = 0; i < num; i++) {
                    //write_loop_buf_cocket(revbuf[i]&0xff);
                    printf("%02X ", revbuf[i]);
                }
                printf("\n");
            } else {
                usleep(1000 * 100);
            }
        } else {
            usleep(1000 * 200);
        }
    }*/
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
/*    sleep(30);
    int rcv_len_, i;
    //重新接收新的一帧
    int rcv_len = 0;
    unsigned char rcv_buf[1024] = {0};
    unsigned char revbuf[1024] = {0};
    unsigned char c;
    while (1) {
        if (access("/tmp/xstrive_upgrade.upf", F_OK) == 0 || (access("/tmp/_4gmodule_found", F_OK) != 0)) {
            sleep(10);
        }

        size_t rcv_buf_len = sizeof(rcv_buf);
        nn_recv(nn_sock_server, &rcv_buf, rcv_buf_len, 0);
        for (size_t i = 0; i < rcv_buf_len; i++) {
            rcv_buf[i] &= 0XFF;
            rcv_len++;
            if (i > 0 && rcv_buf[i] == 0x7E) {
                break;
            }
        }

        net_rcv_ack = 0;//清零
        rcv_len_ = un_transition(revbuf, rcv_buf, rcv_len);//反转译
        printf("--------------------------------------------------------------------------------------------------------------------------------\n");
        printf("receive:%d\n\n", rcv_len_);
        for (i = 0; i < rcv_len; i++) {
            printf("%.2X ", rcv_buf[i]);
        }
        printf("\n\n");
        for (i = 0; i < rcv_len_; i++) {
            printf("%.2X ", revbuf[i]);
        }
        printf("\n");
        printf("--------------------------------------------------------------------------------------------------------------------------------\n");



        //校验
        unsigned char check_num = 0;
        for (i = 1; i < (rcv_len_ - 2); i++) {
            check_num ^= revbuf[i];
        }
        unsigned char check_num_tmp = revbuf[rcv_len_ - 2];
        printf("check_num = %d   check_num_tmp = %d\n", check_num, check_num_tmp);
        if (check_num_tmp == check_num) {//校验不对
            ServerData_Work(revbuf, rcv_len_);
            net_rcv_ack = 0;//清零
            rcv_len = 0;//重新接收新的一帧
        }


        if (MyApplication.NetFlag == 1)//网络数据透传标志
        {
            unsigned char serialBuf[rcv_len_ * 2 + 2];
            memcpy(serialBuf + 1, revbuf, rcv_len_);//复制数据
            ByteToString(serialBuf, rcv_len_ * 2 + 2);
            serialBuf[0] = 0xa1;
            serialBuf[1] = 0xfa;
            SendClientData(0x01, serialBuf, 0x00C0, rcv_len_ * 2 + 2);//透传数据到手机APP(透传数据部分为GBK编码字符串)
        }

        MyApplication.NoACK_sum = 0;//超过10分钟没有服务器数据重启设备
    }*/
}


