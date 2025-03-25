//
// Created by zjw93 on 2025/3/25.
//

#ifndef NDK_COMPILE_SOCKET_H
#define NDK_COMPILE_SOCKET_H


class socket {

};

//jt905 TCP中继服务初始化
int jt905_tcp_relay_init();

//jt905 TCP客户端初始化
int jt905_tcp_relay_init();

void socket_pthread(void *);

void nn_socket_init();


void read_msg(void *arg);

void server_data_worke_thread(void *);

#endif //NDK_COMPILE_SOCKET_H
