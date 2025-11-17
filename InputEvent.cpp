//
// Created by Administrator on 2025/11/11.
//

#include <thread>
#include "InputEvent.h"
#include "ToolKits.h"

void InputEvent::init(const char *device) {
    fd = open(device, O_RDONLY);
}

void InputEvent::handle() {
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
                if (input_event_touch_count == 5) {
                    printf("input_event_touch_count++ %d\r\n", input_event_touch_count);
                    touchEventHandle();//连续点击5次触发电源按键时间
                    input_event_touch_count = 0;
                }
            }

            last_input_event_tv_sec = tv_sec;
            last_input_event_touch_value = value;
        }
    });

    t.detach();
}

void InputEvent::touchEventHandle() {
    if (!ToolKits::isFileExists(RELAY_ON_0_LOCK)) {
        m_srv_sys->broadcast(buf_input_keyevent_power, CAN_BUFFER_LEN);
        std::thread t([]() {
            sleep(1);
            //模拟电源按键
            std::system("input keyevent POWER");
        });
        t.detach();
    }
}

void InputEvent::setSrv(TcpServer *SYS_srv) {
    m_srv_sys = SYS_srv;
}
