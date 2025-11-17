//
// Created by Administrator on 2025/7/4.
//

#include "SerialReceiveHandle.h"
#include "SerialMessageCallback.h"
#include "ToolKits.h"

SerialReceiveHandle::SerialReceiveHandle(Serial *serial, TcpServer *UART0_srv, TcpServer *UART1_srv,
                                         TcpServer *UART2_srv,
                                         TcpServer *UART3_srv)
        : m_serial(serial),
          m_srv_0(UART0_srv), m_srv_1(UART1_srv), m_srv_2(UART2_srv), m_srv_3(UART3_srv) {

}

void SerialReceiveHandle::Serial_receiveHandle() {
    //CAN模块消息线程
    if (m_serial->uart0_fd) {
        std::thread uart0([this]() {
            Serial0_receiveHandle();
        });
        uart0.detach();
    }

    //控制器模块消息线程
    if (m_serial->uart1_fd) {
        std::thread uart1([this]() {
            Serial1_receiveHandle();
        });
        uart1.detach();
    }

    //继电器模块消息线程
    if (m_serial->uart2_fd) {
        std::thread uart2([this]() {
            Serial2_receiveHandle();
        });
        uart2.detach();
    }

    //惯导模块消息线程
    if (m_serial->uart3_fd) {
        std::thread uart3([this]() {
            Serial3_receiveHandle();
        });
        uart3.detach();
    }

}

void SerialReceiveHandle::Serial0_receiveHandle() {
    uint8_t buffer[CAN_BUFFER_LEN]; // 读取CAN模块串口数据
    ssize_t len;
    while (m_serial->uart0_fd) {
        len = m_serial->Serial0_receive(buffer, CAN_BUFFER_LEN);
        if (len < 1)
            continue;
        else if (len == CAN_BUFFER_LEN && buffer[1] == 0x18 && buffer[2] == 0xFF && buffer[3] == 0xFF &&
                 buffer[4] == 0x01 && buffer[5] == 0x01)
            //过滤电池故障CAN扩展帧0x88, 0x18, 0xFF, 0xFF, 0x01, 0x01
            continue;
//        printf("UART0:%zd: ", len);
//        ToolKits::dump(buffer, len);
        m_srv_0->broadcast((const void *) buffer, len); // 转发串口数据到tcp客户端
    }
}

void SerialReceiveHandle::Serial1_receiveHandle() {
    uint8_t buffer[CAN_BUFFER_LEN]; // 读取控制器模块串口数据
    ssize_t len;
    while (m_serial->uart1_fd) {
        len = m_serial->Serial1_receive(buffer, CAN_BUFFER_LEN);
        if (len < 1)
            continue;
//        printf("UART1:%zd: ", len);
//        ToolKits::dump(buffer, len);
        m_srv_1->broadcast((const void *) buffer, len); // 转发串口数据到tcp客户端
    }
}

void SerialReceiveHandle::Serial2_receiveHandle() {
    uint8_t buffer[CAN_BUFFER_LEN]; // 读取继电器串口数据
    ssize_t len;
    while (m_serial->uart2_fd) {
        len = m_serial->Serial2_receive(buffer, CAN_BUFFER_LEN);
        if (len < 1)
            continue;
//        printf("UART2:%zd: ", len);
//        ToolKits::dump(buffer, len);
        m_srv_2->broadcast((const void *) buffer, len); // 转发串口数据到tcp客户端
    }
}

void SerialReceiveHandle::Serial3_receiveHandle() {
    uint8_t buffer[IMU_BUFFER_LEN]; // 读取惯串口数据
    ssize_t len;
    while (m_serial->uart3_fd) {
        len = m_serial->Serial3_receive(buffer, IMU_BUFFER_LEN);
        if (len < 1)
            continue;
//        printf("UART3:%zd: ", len);
//        ToolKits::dump(buffer, len);
        m_srv_3->broadcast((const void *) buffer, len); // 转发串口数据到tcp客户端
    }
}

void SerialReceiveHandle::Serial4_receiveHandle() {
    uint8_t buffer[CAN_BUFFER_LEN]; // 读取串口数据
    ssize_t len;
    while (m_serial->uart4_fd) {
        len = m_serial->Serial4_receive(buffer, CAN_BUFFER_LEN);
        if (len < 1)
            continue;
//        printf("UART4:%zd: \r\n", len);
//        ToolKits::dump(buffer, len);
        m_srv_0->broadcast((const void *) buffer, len); // 转发串口数据到tcp客户端
    }
}

void SerialReceiveHandle::Serial5_receiveHandle() {
    uint8_t buffer[CAN_BUFFER_LEN]; // 读取串口数据
    ssize_t len;
    while (m_serial->uart5_fd) {
        len = m_serial->Serial5_receive(buffer, CAN_BUFFER_LEN);
        if (len < 1)
            continue;
//        printf("UART5:%zd: \r\n", len);
//        ToolKits::dump(buffer, len);
        m_srv_0->broadcast((const void *) buffer, len); // 转发串口数据到tcp客户端
    }
}

void SerialReceiveHandle::Serial6_receiveHandle() {
    uint8_t buffer[CAN_BUFFER_LEN]; // 读取串口数据
    ssize_t len;
    while (m_serial->uart7_fd) {
        len = m_serial->Serial6_receive(buffer, CAN_BUFFER_LEN);
        if (len < 1)
            continue;
//        printf("UART6:%zd: \r\n", len);
//        ToolKits::dump(buffer, len);
        m_srv_0->broadcast((const void *) buffer, len); // 转发串口数据到tcp客户端
    }
}

void SerialReceiveHandle::Serial7_receiveHandle() {
    uint8_t buffer[CAN_BUFFER_LEN]; // 读取串口数据
    ssize_t len;
    while (m_serial->uart7_fd) {
        len = m_serial->Serial7_receive(buffer, CAN_BUFFER_LEN);
        if (len < 1)
            continue;
//        printf("UART7:%zd: ", len);
//        ToolKits::dump(buffer, len);
        m_srv_0->broadcast((const void *) buffer, len); // 转发串口数据到tcp客户端
    }
}
