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

//系统服务端口
#define SYS_PORT 1879
//EC20模块
#define EC20_PATH "/dev/ttyUSB2"
#define EC20_SPEED B9600
//CAN模块
#define UART0_PATH "/dev/ttyCH9344USB0"
#define UART0_SPEED B115200
#define UART0_PORT 1880
//控制器模块
#define UART1_PATH "/dev/ttyCH9344USB1"
#define UART1_SPEED B115200
#define UART1_PORT 1881
//继电器模块
#define UART2_PATH "/dev/ttyCH9344USB2"
#define UART2_SPEED B38400
#define UART2_PORT 1882
//惯导模块
#define UART3_PATH "/dev/ttyCH9344USB3"
#define UART3_SPEED B115200
#define UART3_PORT 1883

#define UART4_PATH "/dev/ttyCH9344USB4"
#define UART4_SPEED B115200
#define UART4_PORT 1884

#define UART5_PATH "/dev/ttyCH9344USB5"
#define UART5_SPEED B115200
#define UART5_PORT 1885

#define UART6_PATH "/dev/ttyCH9344USB6"
#define UART6_SPEED B115200
#define UART6_PORT 1886

#define UART7_PATH "/dev/ttyCH9344USB7"
#define UART7_SPEED B115200
#define UART7_PORT 1887

class Serial {

public:
    void UART_init();

    int UART0_open();

    int UART0_fd();

    int UART0_close();

    ssize_t UART0_receive(void *buf, size_t count);

    ssize_t UART0_send(const void *buf, size_t count);

    ssize_t UART0_sendTest();

    int UART1_open();

    int UART1_fd();

    int UART1_close();

    ssize_t UART1_receive(void *buf, size_t count);

    ssize_t UART1_send(const void *buf, size_t count);

    ssize_t UART1_sendTest();

    int UART2_open();

    int UART2_fd();

    int UART2_close();

    ssize_t UART2_receive(void *buf, size_t count);

    ssize_t UART2_send(const void *buf, size_t count);

    ssize_t UART2_sendTest();

    int UART3_open();

    int UART3_fd();

    int UART3_close();

    ssize_t UART3_receive(void *buf, size_t count);

    ssize_t UART3_send(const void *buf, size_t count);

    ssize_t UART3_sendTest();


    int UART4_open();

    int UART4_fd();

    int UART4_close();

    ssize_t UART4_receive(void *buf, size_t count);

    ssize_t UART4_send(const void *buf, size_t count);

    ssize_t UART4_sendTest();

    int UART5_open();

    int UART5_fd();

    int UART5_close();

    ssize_t UART5_receive(void *buf, size_t count);

    ssize_t UART5_send(const void *buf, size_t count);

    ssize_t UART5_sendTest();

    int UART6_open();

    int UART6_fd();

    int UART6_close();

    ssize_t UART6_receive(void *buf, size_t count);

    ssize_t UART6_send(const void *buf, size_t count);

    ssize_t UART6_sendTest();

    int UART7_open();

    int UART7_fd();

    int UART7_close();

    ssize_t UART7_receive(void *buf, size_t count);

    ssize_t UART7_send(const void *buf, size_t count);

    ssize_t UART7_sendTest();

private:
    /// @brief 打开串口设备
    /// @return
    int UART_open(const char *path, int &fd, int speed);

    /// @brief 关闭串口设备
    /// @return
    int UART_close(int &fd);

    /// @brief 设置串口参数（如波特率等
    /// @param speed
    /// @return
    int UART_setAttribs(int &fd, int speed);

    /// @brief 从串口接收数据
    /// @param buf
    /// @param count
    /// @return
    ssize_t UART_receive(int &fd, void *buf, size_t count);

    /// @brief 向串口发送数据
    /// @param buf
    /// @param count
    /// @return
    ssize_t UART_send(int &fd, const void *buf, size_t count);

private:
    /// @brief 串口文件描述符，打开后用于读写操作
    int uart0_fd = 0;
    int uart1_fd = 0;
    int uart2_fd = 0;
    int uart3_fd = 0;
    int uart4_fd = 0;
    int uart5_fd = 0;
    int uart6_fd = 0;
    int uart7_fd = 0;

    std::mutex uart0_mutex;
    std::mutex uart1_mutex;
    std::mutex uart2_mutex;
    std::mutex uart3_mutex;
    std::mutex uart4_mutex;
    std::mutex uart5_mutex;
    std::mutex uart6_mutex;
    std::mutex uart7_mutex;
};

#endif // TCPSERVER_SERIAL_H
