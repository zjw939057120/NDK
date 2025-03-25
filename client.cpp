//
// Created by zjw93 on 2025/3/25.
//

#include "client.h"

#include <nanomsg/nn.h>
#include <nanomsg/bus.h>
#include <nanomsg/pair.h>
#include <cstdio>

#define MSG_FILE_XSTRIVE "ipc:///tmp/node_xstrive.ipc"
#define MSG_FILE_AV "ipc:///tmp/node_av.ipc"
int nn_client_server;
int nn_client_client;

void nn_client_init() {
    //部署IPC进程间通讯服务端
    nn_client_server = nn_socket(AF_SP, NN_BUS);
    if (nn_client_server < 0) {
        perror("nn_socket");
    }
    if (nn_bind(nn_client_server, MSG_FILE_XSTRIVE) < 0) {  // 绑定自己的地址
        perror("nn_bind");
    }
    //连接IPC进程间通讯客户端
    nn_client_client = nn_socket(AF_SP, NN_BUS);
    if (nn_client_client < 0) {
        perror("nn_socket");
    }
    if (nn_connect(nn_client_client, MSG_FILE_AV) < 0) {  // 连接到服务端
        perror("nn_connect");
    }
}

void jt1078_pthread(void *) {

}
