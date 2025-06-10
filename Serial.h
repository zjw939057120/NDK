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

class Serial {

public:
    /// @brief 打开串口设备
    /// @return 
    int Open();

    /// @brief 关闭串口设备
    /// @return 
    int Close();

    /// @brief 设置串口参数（如波特率等
    /// @param speed 
    /// @return 
    int set_interface_attribs(int speed);

    /// @brief 从串口接收数据
    /// @param buf 
    /// @param count 
    /// @return 
    int Receive(void *buf, size_t count);

    /// @brief 向串口发送数据
    /// @param buf 
    /// @param count 
    /// @return 
    int Send(const void *buf, size_t count);

    /// @brief 向串口发送测试数据
    /// @return 
    int SendTest();

private:
    /// @brief 串口文件描述符，打开后用于读写操作
    int fd = 0;
};

#endif // TCPSERVER_SERIAL_H
