//
// Created by Administrator on 2025/6/10.
//

#include "Verification.h"

Verification::Verification(Serial &serial, TcpServer &srv) : serial(serial), srv(srv) {

}

bool Verification::Check(Buffer *buf) {
    if (buf->size() != CAN_BUFFER_LEN) {
        srv.broadcast(empty, CAN_BUFFER_LEN);
        return false;
    }

    serial.Send(buf->data(), CAN_BUFFER_LEN); // 转发tcp客户端数据到串口
    return true;
}
