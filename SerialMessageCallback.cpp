//
// Created by Administrator on 2025/6/10.
//

#include <iostream>
#include "SerialMessageCallback.h"
#include "ToolKits.h"

SerialMessageCallback::SerialMessageCallback(Serial *serial,
                                             TcpServer *SYS_srv, TcpServer *UART0_srv, TcpServer *UART1_srv,
                                             TcpServer *UART2_srv,
                                             TcpServer *UART3_srv)
        : m_serial(serial),
          m_srv_sys(SYS_srv), m_srv_0(UART0_srv), m_srv_1(UART1_srv), m_srv_2(UART2_srv), m_srv_3(UART3_srv) {

}

void SerialMessageCallback::svr_sys_onMessageCallback(Buffer *buf) {
    size_t size = buf->size();
    auto *buffer = static_cast<uint8_t *>(buf->data());
    //系统消息,校验数据长度
    ToolKits::dump(buffer, size);
    if (size < CAN_BUFFER_LEN) {
        m_srv_sys->broadcast(can_buf_empty, CAN_BUFFER_LEN);
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
        case 0x02: {
            Sys_MsgType_0x02(buffer, msgId, msgBody);
            break;
        }
        case 0x03: {
            Sys_MsgType_0x03(buffer, msgId, msgBody);
            break;
        }
        case 0x10://APP双屏互动消息
        {
            Sys_MsgType_0x10(buffer, msgId, msgBody);
            break;
        }
        case 0x11://APP双屏互动消息
        {
            Sys_MsgType_0x11(buffer, msgId, msgBody);
            break;
        }
        case 0x12://APP双屏互动消息
        {
            Sys_MsgType_0x12(buffer, msgId, msgBody);
            break;
        }
        case 0x13://APP双屏互动消息
        {
            Sys_MsgType_0x13(buffer, msgId, msgBody);
            break;
        }
        default:
            m_srv_sys->broadcast(buffer, CAN_BUFFER_LEN);
            break;
    }
}

void SerialMessageCallback::svr0_onMessageCallback(Buffer *buf) {
    size_t size = buf->size();
    auto *buffer = static_cast<uint8_t *>(buf->data());
    //CAN消息,校验数据长度
    ToolKits::dump(buffer, size);
    m_serial->Serial0_send(buffer, CAN_BUFFER_LEN); // 转发tcp客户端数据到串口
}

void SerialMessageCallback::svr1_onMessageCallback(Buffer *buf) {
    size_t size = buf->size();
    auto *buffer = static_cast<uint8_t *>(buf->data());
    //控制器消息,校验数据长度
    ToolKits::dump(buffer, size);
    m_serial->Serial1_send(buffer, size); // 转发tcp客户端数据到串口
}

void SerialMessageCallback::svr2_onMessageCallback(Buffer *buf) {
    size_t size = buf->size();
    auto *buffer = static_cast<uint8_t *>(buf->data());
    //继电器消息,校验数据长度
    ToolKits::dump(buffer, size);
    if (size != RELAY_BUFFER_LEN) return;

    if (buffer[0] == relay_buf_on_0[0] &&
        buffer[1] == relay_buf_on_0[1] &&
        buffer[2] == relay_buf_on_0[2] &&
        buffer[3] == relay_buf_on_0[3] &&
        buffer[4] == relay_buf_on_0[4] &&
        buffer[5] == relay_buf_on_0[5] &&
        buffer[6] == relay_buf_on_0[6] &&
        buffer[7] == relay_buf_on_0[7]) {
        //打开继电器0号
        ToolKits::createFile(RELAY_ON_0_LOCK);
        // 打开娱乐
        m_srv_sys->broadcast(buf_screen_on, CAN_BUFFER_LEN);
        // 打开中控屏
        ToolKits::systemScreenOn();
    } else if (buffer[0] == relay_buf_off_0[0] &&
               buffer[1] == relay_buf_off_0[1] &&
               buffer[2] == relay_buf_off_0[2] &&
               buffer[3] == relay_buf_off_0[3] &&
               buffer[4] == relay_buf_off_0[4] &&
               buffer[5] == relay_buf_off_0[5] &&
               buffer[6] == relay_buf_off_0[6] &&
               buffer[7] == relay_buf_off_0[7]) {
        //关闭继电器0号
        ToolKits::deleteFile(RELAY_ON_0_LOCK);
        // 关闭娱乐屏
        m_srv_sys->broadcast(buf_screen_off, CAN_BUFFER_LEN);
        // 关闭中控屏
        ToolKits::systemScreenOff();
    }
    m_serial->Serial2_send(buffer, size); // 转发tcp客户端数据到串口
}

void SerialMessageCallback::svr3_onMessageCallback(Buffer *buf) {
    size_t size = buf->size();
    auto *buffer = static_cast<uint8_t *>(buf->data());
    //惯导消息,校验数据长度
    ToolKits::dump(buffer, size);
    m_serial->Serial3_send(buffer, size); // 转发tcp客户端数据到串口
}

void SerialMessageCallback::Sys_MsgType_0x00(uint8_t *buffer, uint32_t msgId, const uint8_t *msgBody) {
    //保存数据
    std::system("sync &");
    m_srv_sys->broadcast(buffer, CAN_BUFFER_LEN);

    switch ((E_SYS_MSG_ID) msgId) {
        case E_SYS_MSG_ID_SETTINGS: {
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
            ToolKits::systemOpenSettings();
            break;
        }
        case E_SYS_MSG_ID_SHUTDOWN: {
            std::thread t([]() {
                std::this_thread::sleep_for(std::chrono::milliseconds(200));
                ToolKits::systemPowerShutown();
            });
            t.detach();
            break;
        }
        case E_SYS_MSG_ID_REBOOT: {
            std::thread t([]() {
                std::this_thread::sleep_for(std::chrono::milliseconds(200));
                ToolKits::systemPowerReboot();
            });
            t.detach();
            break;
        }
        case E_SYS_MSG_ID_REBOOT_BOOTLOADER: {
            std::thread t([]() {
                std::this_thread::sleep_for(std::chrono::milliseconds(200));
                ToolKits::systemPowerRebootLoader();
            });
            t.detach();
            break;
        }
        case E_SYS_MSG_ID_KEYCODE_HOME: {
            std::thread t([]() {
                std::this_thread::sleep_for(std::chrono::milliseconds(200));
                ToolKits::systemKeyCodeHome();
            });
            t.detach();
            break;
        }
        case E_SYS_MSG_ID_KEYCODE_MENU: {
            std::thread t([]() {
                std::this_thread::sleep_for(std::chrono::milliseconds(200));
                ToolKits::systemKeyCodeMenu();
            });
            t.detach();
            break;
        }
        case E_SYS_MSG_ID_KEYCODE_VOLUME_DOWN: {
            std::thread t([]() {
                std::this_thread::sleep_for(std::chrono::milliseconds(200));
                ToolKits::systemKeyCodeVolueDown();
            });
            t.detach();
            break;
        }
        case E_SYS_MSG_ID_KEYCODE_VOLUME_UP: {
            std::thread t([]() {
                std::this_thread::sleep_for(std::chrono::milliseconds(200));
                ToolKits::systemKeyCodeVolueUp();
            });
            t.detach();
            break;
        }
        case E_SYS_MSG_ID_KEYCODE_VOLUME_MUTE: {
            std::thread t([]() {
                std::this_thread::sleep_for(std::chrono::milliseconds(200));
                ToolKits::systemKeyCodeVolueMute();
            });
            t.detach();
            break;
        }
        case E_SYS_MSG_ID_KEYCODE_POWER: {
            std::thread t([]() {
                std::this_thread::sleep_for(std::chrono::milliseconds(200));
                ToolKits::systemKeyCodePower();
            });
            t.detach();
            break;
        }
        case E_SYS_MSG_ID_KEYCODE_SLEEP: {
            std::thread t([]() {
                std::this_thread::sleep_for(std::chrono::milliseconds(200));
                ToolKits::systemKeyCodeSleep();
            });
            t.detach();
            break;
        }
        case E_SYS_MSG_ID_KEYCODE_WAKEUP: {
            std::thread t([]() {
                std::this_thread::sleep_for(std::chrono::milliseconds(200));
                ToolKits::systemKeyCodeWakeup();
            });
            t.detach();
            break;
        }
        case E_SYS_MSG_ID_SCREEN_OFF: {
            std::thread t([]() {
                std::this_thread::sleep_for(std::chrono::milliseconds(200));
                ToolKits::systemScreenOff();
            });
            t.detach();
            break;
        }
        case E_SYS_MSG_ID_SCREEN_ON: {
            std::thread t([]() {
                std::this_thread::sleep_for(std::chrono::milliseconds(200));
                ToolKits::systemScreenOn();
            });
            t.detach();
            break;
        }
        default:
            break;
    }
}

void SerialMessageCallback::Sys_MsgType_0x01(uint8_t *buffer, uint32_t msgId, const uint8_t *msgBody) {
    std::system("sync &");
    m_srv_sys->broadcast(buffer, CAN_BUFFER_LEN);

    switch ((E_SYS_EXT_MSG_ID) msgId) {
        case E_SYS_EXT_MSG_ID_DEMO: {
            //创建demo标识并重启系统
            FILE *fp = fopen(DEMO_LOCK, "w");
            fclose(fp);
            std::thread t([]() {
                sleep(1);
                std::system("svc power reboot");
            });
            t.detach();
            break;
        }
        case E_SYS_EXT_MSG_ID_DEMO_EXIT: {
            //删除demo标识并重启系统
            unlink(DEMO_LOCK);
            std::thread t([]() {
                sleep(1);
                std::system("svc power reboot");
            });
            t.detach();
            break;
        }
        default:
            break;
    }
}


void SerialMessageCallback::Sys_MsgType_0x02(uint8_t *buffer, uint32_t msgId, const uint8_t *msgBody) {
    std::system("sync &");
    m_srv_sys->broadcast(buffer, CAN_BUFFER_LEN);
}

void SerialMessageCallback::Sys_MsgType_0x03(uint8_t *buffer, uint32_t msgId, const uint8_t *msgBody) {
    std::system("sync &");
    m_srv_sys->broadcast(buffer, CAN_BUFFER_LEN);
}

void SerialMessageCallback::Sys_MsgType_0x10(uint8_t *buffer, uint32_t msgId, const uint8_t *msgBody) {
    m_srv_sys->broadcast(buffer, CAN_BUFFER_LEN);
}

void SerialMessageCallback::Sys_MsgType_0x11(uint8_t *buffer, uint32_t msgId, const uint8_t *msgBody) {
    m_srv_sys->broadcast(buffer, CAN_BUFFER_LEN);
}

void SerialMessageCallback::Sys_MsgType_0x12(uint8_t *buffer, uint32_t msgId, const uint8_t *msgBody) {
    m_srv_sys->broadcast(buffer, CAN_BUFFER_LEN);
}

void SerialMessageCallback::Sys_MsgType_0x13(uint8_t *buffer, uint32_t msgId, const uint8_t *msgBody) {
    m_srv_sys->broadcast(buffer, CAN_BUFFER_LEN);
}

void SerialMessageCallback::svr_demoThread() {
    //推进器、电池demo线程
    std::thread t0([this]() {
        while (true) {
            svr0_demoThreadHandle();
        }
    });
    t0.detach();

    //控制器模块demo线程
    std::thread t1([this]() {
        while (true) {
            svr1_demoThreadHandle();
        }
    });
    t1.detach();

    //继电器模块demo线程
    std::thread t2([this]() {
        while (true) {
            svr2_demoThreadHandle();
        }
    });
    t2.detach();

    //惯导模块demo线程
    std::thread t3([this]() {
        while (true) {
            svr3_demoThreadHandle();
        }
    });
    t3.detach();
}

void SerialMessageCallback::svr0_demoThreadHandle() {
    usleep(1000 * 100);
    //模块测试数据
    m_srv_0->broadcast(can_buf_0x200, sizeof(can_buf_0x200));
    sleep(1);
    m_srv_0->broadcast(can_buf_0x300, sizeof(can_buf_0x300));
    sleep(1);
    m_srv_0->broadcast(can_buf_0x301, sizeof(can_buf_0x301));
    sleep(1);
    m_srv_0->broadcast(can_buf_0x18FFFF01_0, sizeof(can_buf_0x18FFFF01_0));
    sleep(1);
    m_srv_0->broadcast(can_buf_0x18FFFF01_1, sizeof(can_buf_0x18FFFF01_1));
    sleep(1);
    m_srv_0->broadcast(can_buf_0x1806E5F4, sizeof(can_buf_0x1806E5F4));
    sleep(1);
}

void SerialMessageCallback::svr1_demoThreadHandle() {
    //模块测试数据
    usleep(1000 * 200);
    m_srv_1->broadcast(can_buf_empty, sizeof(can_buf_empty));
    sleep(10);
}

void SerialMessageCallback::svr2_demoThreadHandle() {
    //模块测试数据
    usleep(1000 * 300);
    m_srv_2->broadcast(relay_buf_0x0103, sizeof(relay_buf_0x0103));
    sleep(10);
}

void SerialMessageCallback::svr3_demoThreadHandle() {
    usleep(1000 * 400);
    //模块测试数据
    m_srv_3->broadcast(imu_buf_0x05, sizeof(imu_buf_0x05));
    sleep(1);
}

void SerialMessageCallback::svr4_demoThreadHandle() {
    //模块测试数据
    usleep(1000 * 500);
    m_srv_0->broadcast(can_buf_empty, sizeof(can_buf_empty));
    sleep(10);
}

void SerialMessageCallback::svr5_demoThreadHandle() {
    //模块测试数据
    usleep(1000 * 600);
    m_srv_0->broadcast(can_buf_empty, sizeof(can_buf_empty));
    sleep(10);
}

void SerialMessageCallback::svr6_demoThreadHandle() {
    //模块测试数据
    usleep(1000 * 700);
    m_srv_0->broadcast(can_buf_empty, sizeof(can_buf_empty));
    sleep(10);
}

void SerialMessageCallback::svr7_demoThreadHandle() {
    //模块测试数据
    usleep(1000 * 800);
    m_srv_0->broadcast(can_buf_empty, sizeof(can_buf_empty));
    sleep(10);
}
