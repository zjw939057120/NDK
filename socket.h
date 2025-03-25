//
// Created by zjw93 on 2025/3/25.
//

#ifndef NDK_COMPILE_SOCKET_H
#define NDK_COMPILE_SOCKET_H


class socket {

};


void socket_pthread(void *);

//jt905 TCP中继服务
int jt905_tcp_relay_server_pthread();

//jt905 TCP客户端线程
int jt905_tcp_client_pthread();


void nn_socket_init();


void read_msg(void *arg);

void server_data_worke_thread(void *);

#endif //NDK_COMPILE_SOCKET_H
