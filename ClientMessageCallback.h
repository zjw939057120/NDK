//
// Created by Administrator on 2025/10/14.
//

#ifndef TCPSERVER_CLIENTMESSAGECALLBACK_H
#define TCPSERVER_CLIENTMESSAGECALLBACK_H

#include "hv/TcpServer.h"

using namespace hv;

class ClientMessageCallback {
public:
    void onMessageCallback(Buffer *buf);

private:
    void Sys_MsgType_0x00(uint8_t *buffer, uint32_t msgId, const uint8_t *msgBody);

    void Sys_MsgType_0x01(uint8_t *buffer, uint32_t msgId, const uint8_t *msgBody);

};


#endif //TCPSERVER_CLIENTMESSAGECALLBACK_H
