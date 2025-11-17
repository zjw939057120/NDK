//
// Created by Administrator on 2025/6/10.
//

#ifndef TCPSERVER_SERIALMESSAGECALLBACK_H
#define TCPSERVER_SERIALMESSAGECALLBACK_H

#include "hv/TcpServer.h"
#include "Serial.h"

//CAN模块缓冲区长度
#define CAN_BUFFER_LEN 13
#define CAN_MSG_ID_LEN 4
#define CAN_MSG_BODY_LEN 8
//继电器模块缓冲区长度
#define RELAY_BUFFER_LEN 8
//惯导模块缓冲区长度
#define IMU_BUFFER_LEN 44

using namespace hv;

class SerialMessageCallback {
public:
    SerialMessageCallback(Serial *serial,
                          TcpServer *SYS_srv, TcpServer *UART0_srv, TcpServer *UART1_srv, TcpServer *UART2_srv,
                          TcpServer *UART3_srv);

    /// @brief  TCP消息处理
    /// @return
    void svr_sys_onMessageCallback(Buffer *buf);

    /// @brief  TCP消息处理
    /// @return
    void svr0_onMessageCallback(Buffer *buf);

    /// @brief  TCP消息处理
    /// @return
    void svr1_onMessageCallback(Buffer *buf);

    /// @brief  TCP消息处理
    /// @return
    void svr2_onMessageCallback(Buffer *buf);

    /// @brief  TCP消息处理
    /// @return
    void svr3_onMessageCallback(Buffer *buf);

    /// @brief  TCP消息处理 系统基本消息
    /// @return
    void Sys_MsgType_0x00(uint8_t *buffer, uint32_t msgId, const uint8_t *msgBody);

    /// @brief  TCP消息处理 系统扩展消息
    /// @return
    void Sys_MsgType_0x01(uint8_t *buffer, uint32_t msgId, const uint8_t *msgBody);

    /// @brief  TCP消息处理
    /// @return
    void Sys_MsgType_0x02(uint8_t *buffer, uint32_t msgId, const uint8_t *msgBody);

    /// @brief  TCP消息处理
    /// @return
    void Sys_MsgType_0x03(uint8_t *buffer, uint32_t msgId, const uint8_t *msgBody);

    /// @brief  TCP消息处理 APP双屏互动消息
    /// @return
    void Sys_MsgType_0x10(uint8_t *buffer, uint32_t msgId, const uint8_t *msgBody);

    /// @brief  TCP消息处理 APP双屏互动消息
    /// @return
    void Sys_MsgType_0x11(uint8_t *buffer, uint32_t msgId, const uint8_t *msgBody);

    /// @brief  TCP消息处理 APP双屏互动消息
    /// @return
    void Sys_MsgType_0x12(uint8_t *buffer, uint32_t msgId, const uint8_t *msgBody);

    /// @brief  TCP消息处理 APP双屏互动消息
    /// @return
    void Sys_MsgType_0x13(uint8_t *buffer, uint32_t msgId, const uint8_t *msgBody);

    void svr_demoThread();

private:
    void svr0_demoThreadHandle();

    void svr1_demoThreadHandle();

    void svr2_demoThreadHandle();

    void svr3_demoThreadHandle();

    void svr4_demoThreadHandle();

    void svr5_demoThreadHandle();

    void svr6_demoThreadHandle();

    void svr7_demoThreadHandle();

private:
    Serial *m_serial;
    TcpServer *m_srv_sys;
    TcpServer *m_srv_0;
    TcpServer *m_srv_1;
    TcpServer *m_srv_2;
    TcpServer *m_srv_3;
};

//系统消息枚举
enum E_SYS_MSG_ID {
    E_SYS_MSG_ID_SETTINGS = 0,//打开设置
    E_SYS_MSG_ID_SHUTDOWN = 1,//关机
    E_SYS_MSG_ID_REBOOT = 2,//重启
    E_SYS_MSG_ID_REBOOT_BOOTLOADER = 3,//进入刷机模式
    E_SYS_MSG_ID_KEYCODE_HOME = 4, //主页按键
    E_SYS_MSG_ID_KEYCODE_MENU = 5, //菜单按键
    E_SYS_MSG_ID_KEYCODE_VOLUME_DOWN = 6, //音量减按键
    E_SYS_MSG_ID_KEYCODE_VOLUME_UP = 7,//音量加按键
    E_SYS_MSG_ID_KEYCODE_VOLUME_MUTE = 8,//静音按键
    E_SYS_MSG_ID_KEYCODE_POWER = 9,//电源按键
    E_SYS_MSG_ID_KEYCODE_SLEEP = 10,//休眠按键
    E_SYS_MSG_ID_KEYCODE_WAKEUP = 11,//唤醒按键
    E_SYS_MSG_ID_SCREEN_OFF = 12,//关闭屏幕
    E_SYS_MSG_ID_SCREEN_ON = 13,//打开屏幕
};

//系统扩展消息枚举
enum E_SYS_EXT_MSG_ID {
    E_SYS_EXT_MSG_ID_DEMO = 0,//进入DEMO模式
    E_SYS_EXT_MSG_ID_DEMO_EXIT = 1,//退出DEMO模式
};

const uint8_t can_buf_empty[CAN_BUFFER_LEN] = {0};
//下发推进器控制报文
const uint8_t can_buf_0x200[CAN_BUFFER_LEN] = {0x08, 0x00, 0x00, 0x02, 0x00, 0x78, 0x0D, 0x78, 0x0D, 0x4C, 0x04,
                                               0x4C, 0x04};
//下发电调0反馈报文
const uint8_t can_buf_0x300[CAN_BUFFER_LEN] = {0x08, 0x00, 0x00, 0x03, 0x00, 0xDF, 0x05, 0x70, 0x00, 0x35, 0x1C,
                                               0x08, 0x00};
//下发电调1反馈报文
const uint8_t can_buf_0x301[CAN_BUFFER_LEN] = {0x08, 0x00, 0x00, 0x03, 0x01, 0xE3, 0x05, 0x77, 0x00, 0x35, 0x1C,
                                               0x08, 0x00};
//下发电池概况信息(扩展帧)
const uint8_t can_buf_0x18FFFF01_0[CAN_BUFFER_LEN] = {0x88, 0x18, 0xFF, 0xFF, 0x01, 0x00, 0x02, 0x0A, 0x00, 0xE4,
                                                      0x22, 0x4B, 0x00};
//下发电池故障信息(扩展帧)
const uint8_t can_buf_0x18FFFF01_1[CAN_BUFFER_LEN] = {0x88, 0x18, 0xFF, 0xFF, 0x01, 0x01, 0x00, 0x00, 0x07, 0x10,
                                                      0x02, 0xB8, 0x00};
//下发充电机接收信息(扩展帧)
const uint8_t can_buf_0x1806E5F4[CAN_BUFFER_LEN] = {0x88, 0x18, 0x06, 0xE5, 0xF4, 0x02, 0x48, 0x08, 0xFC, 0x00,
                                                    0x00, 0x00, 0x00};

//下发继电器状态
const uint8_t relay_buf_0x0103[21] = {0x01, 0x03, 0x10, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00,
                                      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2E, 0xB4};
//打开继电器0号
const uint8_t relay_buf_on_0[RELAY_BUFFER_LEN] = {0x01, 0x06, 0x00, 0x00, 0x00, 0x01, 0x48, 0x0A};

//关闭继电器0号
const uint8_t relay_buf_off_0[RELAY_BUFFER_LEN] = {0x01, 0x06, 0x00, 0x00, 0x00, 0x00, 0x89, 0xCA};

//关闭娱乐屏幕E_SYS_MSG_ID_SCREEN_OFF
const uint8_t buf_screen_off[CAN_BUFFER_LEN] = {0x00, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x00,
                                                0x00, 0x00, 0x00, 0x00};
//打开娱乐屏幕E_SYS_MSG_ID_SCREEN_ON
const uint8_t buf_screen_on[CAN_BUFFER_LEN] = {0x00, 0x00, 0x00, 0x00, 0x0D, 0x00, 0x00, 0x00, 0x00,
                                               0x00, 0x00, 0x00, 0x00};

//惯导模块消息(11字节加速度+11字节角速度+11字节角度+11字节磁场)
const uint8_t imu_buf_0x05[IMU_BUFFER_LEN] = {0x55, 0x51, 0x4b, 0x00, 0x0a, 0x00, 0xff, 0x07, 0x8b, 0x0b, 0x9a,
                                              0x55, 0x52, 0xe5, 0xff, 0x3e, 0x00, 0x00, 0x00, 0x8b, 0x0b, 0x5f,
                                              0x55, 0x53, 0xf8, 0x00, 0x84, 0xfe, 0x6e, 0x0a, 0xfb, 0x46, 0xde,
                                              0x55, 0x54, 0x83, 0x0c, 0x43, 0x26, 0x98, 0xe7, 0x00, 0x00, 0x20
};

#endif // TCPSERVER_SERIALMESSAGECALLBACK_H
