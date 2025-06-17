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

    void demoThread();

    void demoThreadHandle();

private:
    Serial &m_serial;
    TcpServer &m_srv;
    const uint8_t buf_empty[CAN_BUFFER_LEN] = {0};
    //推进器控制报文
    const uint8_t buf_0x200[CAN_BUFFER_LEN] = {0x08, 0x00, 0x00, 0x02, 0x00, 0x78, 0x0D, 0x78, 0x0D, 0x4C, 0x04, 0x4C,
                                               0x04};
    //电调0反馈报文
    const uint8_t buf_0x300[CAN_BUFFER_LEN] = {0x08, 0x00, 0x00, 0x03, 0x00, 0xDF, 0x05, 0x70, 0x00, 0x35, 0x1C, 0x08,
                                               0x00};
    //电调1反馈报文
    const uint8_t buf_0x301[CAN_BUFFER_LEN] = {0x08, 0x00, 0x00, 0x03, 0x01, 0xE3, 0x05, 0x77, 0x00, 0x35, 0x1C, 0x08,
                                               0x00};
};

#endif // TCPSERVER_VERIFICATION_H
