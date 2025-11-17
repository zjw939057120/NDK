//
// Created by Administrator on 2025/11/11.
//

#include <thread>
#include "Daemon.h"
#include "ToolKits.h"

void Daemon::init(const char *device) {
    fd = open(device, O_RDONLY);
}

void Daemon::touchEventThread() {
    if (!fd) return;

    std::thread t([this]() {
        struct input_event ev;
        while ((read(fd, &ev, sizeof(ev))) == sizeof(ev)) {
            if (ev.code != BTN_TOUCH) continue;//过滤非触屏事件

            auto value = (E_INPUT_EVENT_TOUCH) ev.value;
            auto tv_sec = ev.time.tv_sec;
            if (last_input_event_tv_sec > 0 && last_input_event_tv_sec + 2 <= tv_sec) {
//                printf("continue %x %ld\r\n", value, tv_sec);
                //长时间未触摸后的第一次触摸down
                input_event_touch_count = 0;//重置
                last_input_event_touch_value = E_INPUT_EVENT_TOUCH_UP;//重置
                last_input_event_tv_sec = 0;//重置
                continue;//过滤非连续点击事件
            } else if (last_input_event_touch_value == E_INPUT_EVENT_TOUCH_UP && value == E_INPUT_EVENT_TOUCH_UP) {
//                printf("continue %x %ld\r\n", value, tv_sec);
                //长时间未触摸后的第一次触摸up
                input_event_touch_count = 0;//重置
                last_input_event_touch_value = E_INPUT_EVENT_TOUCH_UP;//重置
                last_input_event_tv_sec = 0;//重置
                continue;//过滤非连续点击事件
            } else if (last_input_event_touch_value == E_INPUT_EVENT_TOUCH_DOWN && value == E_INPUT_EVENT_TOUCH_UP) {
                input_event_touch_count++;//连续触屏计数
//                printf("input_event_touch_count++ %d\r\n", input_event_touch_count);
                if (input_event_touch_count == 5) {
                    printf("input_event_touch_count = %d\r\n", input_event_touch_count);
                    input_event_touch_count = 0;
                    if (!ToolKits::isFileExists(SCREEN_ON_LOCK)) {
                        //强制打开屏幕
                        m_srv_sys->broadcast(buf_screen_on, CAN_BUFFER_LEN);
                        ToolKits::systemScreenOn(true);
                    }
                }
            }

            last_input_event_tv_sec = tv_sec;
            last_input_event_touch_value = value;
        }
    });
    t.detach();
}

void Daemon::setSrv(TcpServer *SYS_srv) {
    m_srv_sys = SYS_srv;
}

void Daemon::screenCheckThread() {
    if (!fd) return;

    std::thread t([this]() {
        while (true) {
            sleep(5 * 60);//5分钟检测屏幕状态
            if (ToolKits::isFileExists(RELAY_ON_0_LOCK)) {
                m_srv_sys->broadcast(buf_screen_on, CAN_BUFFER_LEN);
                ToolKits::systemScreenOn();
            } else {
                m_srv_sys->broadcast(buf_screen_off, CAN_BUFFER_LEN);
                ToolKits::systemScreenOff();
            }
        }
    });
    t.detach();
}

