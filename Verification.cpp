//
// Created by Administrator on 2025/6/10.
//

#include <iostream>
#include "Verification.h"
#include "ToolKits.h"

Verification::Verification(Serial &serial, TcpServer &srv) : m_serial(serial), m_srv(srv) {

}

bool Verification::onMessage(Buffer *buf) {
    size_t size = buf->size();
    auto *buffer = static_cast<uint8_t *>(buf->data());
    //校验数据长度
    ToolKits::dump(buffer, size);
    if (size < CAN_BUFFER_LEN) {
        m_srv.broadcast(can_buf_empty, CAN_BUFFER_LEN);
        return false;
    }

    //消息类型
    uint8_t msgType = buffer[0];
    switch (msgType) {
        case 0x01://系统消息
        {
            uint32_t tmp = 0;
            memcpy(&tmp, &buffer[1], CAN_MSG_ID_LEN);
            //消息ID
            uint32_t msgId = ntohl(tmp); // 大端序转小端序、
            //消息
            uint8_t msgBody[8];
            memcpy(&msgBody, &buffer[5], CAN_MSG_BODY_LEN);
            printf("msgType:0x%02x,msgId:0x%02x\n", msgType, msgId);
            MsgType_0x01(buffer, msgId, msgBody);
        }
            break;
        case 0x02://MCU消息
            m_serial.UART1_send(buf->data(), CAN_BUFFER_LEN); // 转发tcp客户端数据到MCU串口
            break;
        case 0x08://CAN标准帧
        case 0x88://CAN扩展帧
            m_serial.UART0_send(buf->data(), CAN_BUFFER_LEN); // 转发tcp客户端数据到串口
            break;
        default:
            break;
    }
    return true;
}

bool Verification::MsgType_0x01(uint8_t *buffer, uint32_t msgId, const uint8_t *msgBody) {
    switch (msgId) {
        case 0x00:
            //预留
            break;
        case 0x01: {
            //写入继电器状态
            msgBody[0] == 1 ? ToolKits::GPIOSetHigh(SWITCH_1) : ToolKits::GPIOSetLow(SWITCH_1);
            msgBody[1] == 1 ? ToolKits::GPIOSetHigh(SWITCH_2) : ToolKits::GPIOSetLow(SWITCH_2);
            msgBody[2] == 1 ? ToolKits::GPIOSetHigh(SWITCH_3) : ToolKits::GPIOSetLow(SWITCH_3);
            msgBody[3] == 1 ? ToolKits::GPIOSetHigh(SWITCH_4) : ToolKits::GPIOSetLow(SWITCH_4);
            msgBody[4] == 1 ? ToolKits::GPIOSetHigh(SWITCH_5) : ToolKits::GPIOSetLow(SWITCH_5);
            msgBody[5] == 1 ? ToolKits::GPIOSetHigh(SWITCH_6) : ToolKits::GPIOSetLow(SWITCH_6);

            //读取继电器状态
            uint8_t body[CAN_MSG_BODY_LEN] = {0};
            uint8_t index = 0;
            body[index] = ToolKits::GPIOGetValue(SWITCH_1);
            index++;
            body[index] = ToolKits::GPIOGetValue(SWITCH_2);
            index++;
            body[index] = ToolKits::GPIOGetValue(SWITCH_3);
            index++;
            body[index] = ToolKits::GPIOGetValue(SWITCH_4);
            index++;
            body[index] = ToolKits::GPIOGetValue(SWITCH_5);
            index++;
            body[index] = ToolKits::GPIOGetValue(SWITCH_6);
            memcpy(&buffer[CAN_MSG_ID_LEN + 1], body, CAN_MSG_BODY_LEN);
            m_srv.broadcast(buffer, CAN_BUFFER_LEN);
            break;
        }
        case 0x02: {
            //读取继电器状态
            uint8_t body[CAN_MSG_BODY_LEN] = {0};
            uint8_t index = 0;
            body[index] = ToolKits::GPIOGetValue(SWITCH_1);
            index++;
            body[index] = ToolKits::GPIOGetValue(SWITCH_2);
            index++;
            body[index] = ToolKits::GPIOGetValue(SWITCH_3);
            index++;
            body[index] = ToolKits::GPIOGetValue(SWITCH_4);
            index++;
            body[index] = ToolKits::GPIOGetValue(SWITCH_5);
            index++;
            body[index] = ToolKits::GPIOGetValue(SWITCH_6);
            memcpy(&buffer[CAN_MSG_ID_LEN + 1], body, CAN_MSG_BODY_LEN);
            m_srv.broadcast(buffer, CAN_BUFFER_LEN);
            break;
        }
    }
    return true;
}

void Verification::demoThread() {
    //推进器demo线程
    std::thread t1([this]() {
        while (true) {
            demoThreadHandle1();
        }
    });
    t1.detach();

    //电池管理demo线程
    std::thread t2([this]() {
        while (true) {
            demoThreadHandle2();
        }
    });
    t2.detach();
}

void Verification::demoThreadHandle1() {
    usleep(1000 * 500);
    m_srv.broadcast(can_buf_0x200, CAN_BUFFER_LEN);
    sleep(1);
    m_srv.broadcast(can_buf_0x300, CAN_BUFFER_LEN);
    sleep(1);
    m_srv.broadcast(can_buf_0x301, CAN_BUFFER_LEN);
}

void Verification::demoThreadHandle2() {
    usleep(1000 * 1000);
    m_srv.broadcast(can_buf_0x18FFFF01_0, CAN_BUFFER_LEN);
    sleep(1);
    m_srv.broadcast(can_buf_0x18FFFF01_1, CAN_BUFFER_LEN);
    sleep(1);
    m_srv.broadcast(can_buf_0x1806E5F4, CAN_BUFFER_LEN);
}

void Verification::releaseThread() {
    //CAN消息线程
    std::thread t1([this]() {
        while (true) {
            UART0_receiveThreadHandle();
        }
    });
    t1.detach();

    //MCU消息线程
    std::thread t2([this]() {
        while (true) {
            UART1_receiveThreadHandle();
        }
    });
    //等待线程结束
    t2.join();
}

void Verification::UART0_receiveThreadHandle() {
    uint8_t buffer[CAN_BUFFER_LEN]; // 读取CAN串口数据
    ssize_t len = 0;
    while (true) {
        len = m_serial.UART0_receive(buffer, CAN_BUFFER_LEN);
        ToolKits::dump(buffer, len);
        if (len == CAN_BUFFER_LEN) {
            m_srv.broadcast((const void *) buffer, CAN_BUFFER_LEN); // 转发串口数据到tcp客户端
        }
    }
}

void Verification::UART1_receiveThreadHandle() {
    uint8_t buffer[CAN_BUFFER_LEN]; // 读取MCU串口数据
    ssize_t len = 0;
    while (true) {
        len = m_serial.UART1_receive(buffer, CAN_BUFFER_LEN);
        ToolKits::dump(buffer, len);
        if (len == CAN_BUFFER_LEN) {
            m_srv.broadcast((const void *) buffer, CAN_BUFFER_LEN); // 转发串口数据到tcp客户端
        }
    }
}