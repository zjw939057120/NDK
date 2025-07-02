//
// Created by Administrator on 2025/6/10.
//

#include "Serial.h"

#ifndef TCPSERVER_VERIFICATION_H
#define TCPSERVER_VERIFICATION_H

#include "hv/TcpServer.h"
#include "Serial.h"

//CAN模块缓冲区长度
#define CAN_BUFFER_LEN 13
#define CAN_MSG_ID_LEN 4
#define CAN_MSG_BODY_LEN 8
//惯导模块缓冲区长度
#define IMU_BUFFER_LEN 11

using namespace hv;

class Verification {
public:
    Verification(Serial &serial, TcpServer &srv);

    /// @brief  TCP消息处理
    /// @return 
    bool onMessage(Buffer *buf);

    bool MsgType_0x01(uint8_t *buffer, uint32_t msgId, const uint8_t *msgBody);

    void demoThread();

    void demoThreadHandle1();

    void demoThreadHandle2();

    void releaseThread();

    void UART0_receiveThreadHandle();

    void UART1_receiveThreadHandle();

    void UART2_receiveThreadHandle();

    void UART3_receiveThreadHandle();

    void UART4_receiveThreadHandle();

    void UART5_receiveThreadHandle();

    void UART6_receiveThreadHandle();

    void UART7_receiveThreadHandle();

private:
    Serial &m_serial;
    TcpServer &m_srv;
    const uint8_t can_buf_empty[CAN_BUFFER_LEN] = {0};
    //推进器控制报文
    const uint8_t can_buf_0x200[CAN_BUFFER_LEN] = {0x08, 0x00, 0x00, 0x02, 0x00, 0x78, 0x0D, 0x78, 0x0D, 0x4C, 0x04,
                                                   0x4C, 0x04};
    //电调0反馈报文
    const uint8_t can_buf_0x300[CAN_BUFFER_LEN] = {0x08, 0x00, 0x00, 0x03, 0x00, 0xDF, 0x05, 0x70, 0x00, 0x35, 0x1C,
                                                   0x08, 0x00};
    //电调1反馈报文
    const uint8_t can_buf_0x301[CAN_BUFFER_LEN] = {0x08, 0x00, 0x00, 0x03, 0x01, 0xE3, 0x05, 0x77, 0x00, 0x35, 0x1C,
                                                   0x08, 0x00};
    //电池概况信息(扩展帧)
    const uint8_t can_buf_0x18FFFF01_0[CAN_BUFFER_LEN] = {0x88, 0x18, 0xFF, 0xFF, 0x01, 0x00, 0x02, 0x0A, 0x00, 0xE4,
                                                          0x22, 0x4B, 0x00};
    //电池故障信息(扩展帧)
    const uint8_t can_buf_0x18FFFF01_1[CAN_BUFFER_LEN] = {0x88, 0x18, 0xFF, 0xFF, 0x01, 0x01, 0x00, 0x00, 0x07, 0x10,
                                                          0x02, 0xB8, 0x00};
    //充电机接收信息(扩展帧)
    const uint8_t can_buf_0x1806E5F4[CAN_BUFFER_LEN] = {0x88, 0x18, 0x06, 0xE5, 0xF4, 0x02, 0x48, 0x08, 0xFC, 0x00,
                                                        0x00, 0x00, 0x00};

    //惯导模块
    const uint8_t imu_buf_0x0551[IMU_BUFFER_LEN] = {0x55, 0x51, 0xdb, 0x00, 0x19, 0x00, 0xf8, 0x07, 0x5f, 0x0b, 0x03};
};

#endif // TCPSERVER_VERIFICATION_H
