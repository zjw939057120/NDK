//
// Created by Administrator on 2025/7/4.
//

#ifndef TCPSERVER_SERIALRECEIVEHANDLE_H
#define TCPSERVER_SERIALRECEIVEHANDLE_H

#include "hv/TcpServer.h"
#include "Serial.h"

using namespace hv;

class SerialReceiveHandle {

public:
    SerialReceiveHandle(Serial &serial,
                        TcpServer &UART0_srv, TcpServer &UART1_srv, TcpServer &UART2_srv, TcpServer &UART3_srv);

    void Serial_receiveHandle();

private:
    void Serial0_receiveHandle();

    void Serial1_receiveHandle();

    void Serial2_receiveHandle();

    void Serial3_receiveHandle();

    void Serial4_receiveHandle();

    void Serial5_receiveHandle();

    void Serial6_receiveHandle();

    void Serial7_receiveHandle();

private:
    Serial &m_serial;
    TcpServer &m_srv_0;
    TcpServer &m_srv_1;
    TcpServer &m_srv_2;
    TcpServer &m_srv_3;

};


#endif //TCPSERVER_SERIALRECEIVEHANDLE_H
