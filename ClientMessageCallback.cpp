//
// Created by Administrator on 2025/10/14.
//

#include "ClientMessageCallback.h"
#include "Verification.h"
#include "ToolKits.h"

void ClientMessageCallback::onMessageCallback(Buffer *buf) {
    size_t size = buf->size();
    auto *buffer = static_cast<uint8_t *>(buf->data());
    //系统消息,校验数据长度
    ToolKits::dump(buffer, size);
    if (size < CAN_BUFFER_LEN) {
        return;
    }

    //消息类型
    uint8_t msgType = buffer[0];
    uint32_t msgId_tmp = 0;
    memcpy(&msgId_tmp, &buffer[1], CAN_MSG_ID_LEN);
    //消息ID
    uint32_t msgId = ntohl(msgId_tmp); // 大端序转小端序、
    //消息
    uint8_t msgBody[8];
    memcpy(&msgBody, &buffer[5], CAN_MSG_BODY_LEN);
    printf("msgType:0x%02x,msgId:0x%02x\n", msgType, msgId);

    switch (msgType) {
        case 0x00://系统基本消息
        {
            Sys_MsgType_0x00(buffer, msgId, msgBody);
            break;
        }
        case 0x01://系统扩展消息
        {
            Sys_MsgType_0x01(buffer, msgId, msgBody);
            break;
        }
        default:
            break;
    }
}

void ClientMessageCallback::Sys_MsgType_0x00(uint8_t *buffer, uint32_t msgId, const uint8_t *msgBody) {
    std::system("sync &");

    switch ((E_SYS_MSG_ID) msgId) {
        case E_SYS_MSG_ID_SETTINGS: {
            std::system("am start -a android.settings.SETTINGS &");
            break;
        }
        case E_SYS_MSG_ID_SHUTDOWN: {
            std::system("svc power shutdown &");
            break;
        }
        case E_SYS_MSG_ID_REBOOT: {
            std::system("svc power reboot &");
            break;
        }
        case E_SYS_MSG_ID_REBOOT_BOOTLOADER: {
            std::system("svc power reboot loader &");
            break;
        }
        default:
            break;
    }
}

void ClientMessageCallback::Sys_MsgType_0x01(uint8_t *buffer, uint32_t msgId, const uint8_t *msgBody) {
    std::system("sync &");
}
