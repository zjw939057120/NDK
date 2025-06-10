//
// Created by Administrator on 2025/6/10.
//

#include "Serial.h"

#ifndef TCPSERVER_VERIFICATION_H
#define TCPSERVER_VERIFICATION_H

#include "hv/TcpServer.h"
#include "Serial.h"

#define CAN_BUFFER_LEN 13
#define CAN_MSG_TYPE_LEN 1
#define CAN_MSG_ID_LEN 4
#define CAN_MSG_BODY_LEN 8
using namespace hv;

class Verification {
public:
    Verification(Serial &serial, TcpServer &srv);

    /// @brief  验证
    /// @return 
    bool Check(Buffer *buf);

    bool HandleMsg_0x01(uint8_t *buffer, uint32_t msgId, const uint8_t *msgBody);

private:
    Serial &serial;
    TcpServer &srv;
    const uint8_t empty[CAN_BUFFER_LEN] = {0};
};

#endif // TCPSERVER_VERIFICATION_H
