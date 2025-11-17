//
// Created by Administrator on 2025/11/11.
//

#ifndef TCPSERVER_DAEMON_H
#define TCPSERVER_DAEMON_H

#include <cstdio>
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>
#include <linux/types.h>
#include <linux/input.h>

#include "hv/TcpServer.h"
#include "hv/TcpClient.h"
#include "hv/htime.h"
#include "SerialMessageCallback.h"

using namespace hv;

enum E_INPUT_EVENT_TOUCH {
    E_INPUT_EVENT_TOUCH_UP = 0,
    E_INPUT_EVENT_TOUCH_DOWN = 1,
};//触屏动作

class Daemon {

public:
    void init(const char *device);

    void setSrv(TcpServer *SYS_srv);

    void touchEventThread();

    void screenCheckThread();

private:
    TcpServer *m_srv_sys = NULL;
    int fd = 0;//句柄
    uint8_t input_event_touch_count = 0;//连续触屏计数
    long last_input_event_tv_sec = 0;//上一次触屏时间
    E_INPUT_EVENT_TOUCH last_input_event_touch_value = E_INPUT_EVENT_TOUCH_UP;//上一次触屏值

    //关闭娱乐屏幕E_SYS_MSG_ID_SCREEN_OFF
    const uint8_t buf_screen_off[CAN_BUFFER_LEN] = {0x00, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x00,
                                                    0x00, 0x00, 0x00, 0x00};
    //打开娱乐屏幕E_SYS_MSG_ID_SCREEN_ON
    const uint8_t buf_screen_on[CAN_BUFFER_LEN] = {0x00, 0x00, 0x00, 0x00, 0x0D, 0x00, 0x00, 0x00, 0x00,
                                                   0x00, 0x00, 0x00, 0x00};
};


#endif //TCPSERVER_DAEMON_H
