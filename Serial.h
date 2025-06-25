//
// Created by Administrator on 2025/6/9.
//

#ifndef TCPSERVER_SERIAL_H
#define TCPSERVER_SERIAL_H

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <termios.h>
#include <mutex>

class Serial {

public:
    /// @brief 打开串口设备
    /// @return 
    int CANopen();

    /// @brief 关闭串口设备
    /// @return 
    int CANclose();

    /// @brief 设置串口参数（如波特率等
    /// @param speed 
    /// @return 
    int CANsetAttribs(int speed);

    /// @brief 从串口接收数据
    /// @param buf 
    /// @param count 
    /// @return 
    ssize_t CANreceive(void *buf, size_t count);

    /// @brief 向串口发送数据
    /// @param buf 
    /// @param count 
    /// @return 
    ssize_t CANsend(const void *buf, size_t count);

    /// @brief 向串口发送测试数据
    /// @return 
    ssize_t CANsendTest();

    int MCUopen();

    int MCUclose();

    int MCUsetAttribs(int speed);

    ssize_t MCUreceive(void *buf, size_t count);

    ssize_t MCUsend(const void *buf, size_t count);

    ssize_t MCUsendTest();

private:
    /// @brief 串口文件描述符，打开后用于读写操作
    int CANfd = 0;
    int MCUfd = 0;

    std::mutex m_can_send_mutex;
    std::mutex m_mcu_send_mutex;
};

#endif // TCPSERVER_SERIAL_H
