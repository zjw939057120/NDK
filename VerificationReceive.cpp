//
// Created by Administrator on 2025/7/4.
//

#include "VerificationReceive.h"
#include "Verification.h"
#include "ToolKits.h"

VerificationReceive::VerificationReceive(Serial &serial, TcpServer &UART0_srv, TcpServer &UART1_srv,
                                         TcpServer &UART2_srv,
                                         TcpServer &UART3_srv)
        : m_serial(serial),
          m_srv_0(UART0_srv), m_srv_1(UART1_srv), m_srv_2(UART2_srv), m_srv_3(UART3_srv) {

}

void VerificationReceive::UART_receiveThread() {
    //CAN模块消息线程
    if (m_serial.UART0_fd()) {
        std::thread uart0([this]() {
            while (true) {
                UART0_receiveThreadHandle();
            }
        });
        uart0.detach();
    }

    //控制器模块消息线程
    if (m_serial.UART1_fd()) {
        std::thread uart1([this]() {
            while (true) {
                UART1_receiveThreadHandle();
            }
        });
        uart1.detach();
    }

    //继电器模块消息线程
    if (m_serial.UART2_fd()) {
        std::thread uart2([this]() {
            while (true) {
                UART2_receiveThreadHandle();
            }
        });
        uart2.detach();
    }

    //惯导模块消息线程
    if (m_serial.UART3_fd()) {
        std::thread uart3([this]() {
            while (true) {
                UART3_receiveThreadHandle();
            }
        });
        uart3.detach();
    }

}

void VerificationReceive::UART0_receiveThreadHandle() {
    uint8_t buffer[CAN_BUFFER_LEN]; // 读取CAN模块串口数据
    ssize_t len;
    while (true) {
        len = m_serial.UART0_receive(buffer, CAN_BUFFER_LEN);
        printf("UART0:%zd: ", len);
        ToolKits::dump(buffer, len);
        m_srv_0.broadcast((const void *) buffer, len); // 转发串口数据到tcp客户端
    }
}

void VerificationReceive::UART1_receiveThreadHandle() {
    uint8_t buffer[CAN_BUFFER_LEN]; // 读取控制器模块串口数据
    ssize_t len;
    while (true) {
        len = m_serial.UART1_receive(buffer, CAN_BUFFER_LEN);
        printf("UART1:%zd: ", len);
        ToolKits::dump(buffer, len);
        m_srv_1.broadcast((const void *) buffer, len); // 转发串口数据到tcp客户端
    }
}

void VerificationReceive::UART2_receiveThreadHandle() {
    uint8_t buffer[CAN_BUFFER_LEN]; // 读取继电器串口数据
    ssize_t len;
    while (true) {
        len = m_serial.UART2_receive(buffer, CAN_BUFFER_LEN);
        printf("UART2:%zd: ", len);
        ToolKits::dump(buffer, len);
        m_srv_2.broadcast((const void *) buffer, len); // 转发串口数据到tcp客户端
    }
}

void VerificationReceive::UART3_receiveThreadHandle() {
    uint8_t buffer[IMU_BUFFER_LEN]; // 读取惯串口数据
    ssize_t len;
    while (true) {
        len = m_serial.UART3_receive(buffer, IMU_BUFFER_LEN);
        printf("UART3:%zd: ", len);
        ToolKits::dump(buffer, len);
        m_srv_3.broadcast((const void *) buffer, len); // 转发串口数据到tcp客户端
    }
}

void VerificationReceive::UART4_receiveThreadHandle() {
    uint8_t buffer[CAN_BUFFER_LEN]; // 读取串口数据
    ssize_t len;
    while (true) {
        len = m_serial.UART4_receive(buffer, CAN_BUFFER_LEN);
        printf("UART4:%zd: \r\n", len);
        ToolKits::dump(buffer, len);
        m_srv_0.broadcast((const void *) buffer, len); // 转发串口数据到tcp客户端
    }
}

void VerificationReceive::UART5_receiveThreadHandle() {
    uint8_t buffer[CAN_BUFFER_LEN]; // 读取串口数据
    ssize_t len;
    while (true) {
        len = m_serial.UART5_receive(buffer, CAN_BUFFER_LEN);
        printf("UART5:%zd: \r\n", len);
        ToolKits::dump(buffer, len);
        m_srv_0.broadcast((const void *) buffer, len); // 转发串口数据到tcp客户端
    }
}

void VerificationReceive::UART6_receiveThreadHandle() {
    uint8_t buffer[CAN_BUFFER_LEN]; // 读取串口数据
    ssize_t len;
    while (true) {
        len = m_serial.UART6_receive(buffer, CAN_BUFFER_LEN);
        printf("UART6:%zd: \r\n", len);
        ToolKits::dump(buffer, len);
        m_srv_0.broadcast((const void *) buffer, len); // 转发串口数据到tcp客户端
    }
}

void VerificationReceive::UART7_receiveThreadHandle() {
    uint8_t buffer[CAN_BUFFER_LEN]; // 读取串口数据
    ssize_t len;
    while (true) {
        len = m_serial.UART7_receive(buffer, CAN_BUFFER_LEN);
        printf("UART7:%zd: ", len);
        ToolKits::dump(buffer, len);
        m_srv_0.broadcast((const void *) buffer, len); // 转发串口数据到tcp客户端
    }
}