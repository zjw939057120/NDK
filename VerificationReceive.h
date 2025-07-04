//
// Created by Administrator on 2025/7/4.
//

#ifndef TCPSERVER_VERIFICATIONRECEIVE_H
#define TCPSERVER_VERIFICATIONRECEIVE_H

#include "hv/TcpServer.h"
#include "Serial.h"

using namespace hv;

class VerificationReceive {

public:
    VerificationReceive(Serial &serial,
                        TcpServer &UART0_srv, TcpServer &UART1_srv, TcpServer &UART2_srv, TcpServer &UART3_srv);

    void UART_receiveThread();

private:
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
    TcpServer &m_srv_0;
    TcpServer &m_srv_1;
    TcpServer &m_srv_2;
    TcpServer &m_srv_3;

};


#endif //TCPSERVER_VERIFICATIONRECEIVE_H
