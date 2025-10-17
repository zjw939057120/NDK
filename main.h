//
// Created by Administrator on 2025/10/17.
//

#ifndef TCPSERVER_MAIN_H
#define TCPSERVER_MAIN_H

#include "hv/TcpServer.h"
#include "hv/TcpClient.h"
#include "hv/htime.h"
#include "Serial.h"
#include "Verification.h"
#include "VerificationReceive.h"
#include "ToolKits.h"
#include "ClientMessageCallback.h"

using namespace hv;

#define TEST_RECONNECT  1
#define TEST_TLS 0

int UART_TcpServer_Instance(TcpServer &srv, int port, const std::function<void(Buffer *)> &onMessage);

int TCP_Client_Instance(TcpClient &cli, int remote_port, const char *remote_host, const std::function<void(Buffer *)> &onMessage);

void init();

#endif //TCPSERVER_MAIN_H
