//
// Created by Administrator on 2025/6/9.
//

#include "Serial.h"

int Serial::CANopen() {
    const char *portname = "/dev/ttyS3"; // 替换为你自己的串口设备
    CANfd = open(portname, O_RDWR | O_NOCTTY | O_SYNC);
    if (CANfd < 0) {
        perror("open m_serial port");
        return 1;
    }

    if (CANsetAttribs(B115200) < 0) { // 设置波特率为115200
        fprintf(stderr, "Error setting attributes\n");
        close(CANfd);
        return 1;
    }
    return 0;
}

int Serial::CANclose() {
    close(CANfd);
    return 0;
}

int Serial::CANsetAttribs(int speed) {
    struct termios tty;

    if (tcgetattr(CANfd, &tty) != 0) {
        perror("tcgetattr");
        return -1;
    }

    cfsetospeed(&tty, speed);
    cfsetispeed(&tty, speed);

    tty.c_cflag |= (CLOCAL | CREAD); // 启用接收器
    tty.c_cflag &= ~PARENB;          // 无校验
    tty.c_cflag &= ~CSTOPB;          // 1个停止位
    tty.c_cflag &= ~CSIZE;           // 清除数据位掩码
    tty.c_cflag |= CS8;              // 8个数据位
    tty.c_cflag &= ~CRTSCTS;         // 无硬件流控制

    tty.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); // 非规范模式
    tty.c_oflag &= ~OPOST;                          // 原始输出

    tty.c_cc[VMIN] = 1;   // 读取至少1个字符
    tty.c_cc[VTIME] = 10; // 等待时间（单位为 0.1 秒）

    if (tcsetattr(CANfd, TCSANOW, &tty) != 0) {
        perror("tcsetattr");
        return -1;
    }

    return 0;
}

ssize_t Serial::CANreceive(void *buf, size_t count) {
    return read(CANfd, buf, count); // 读取数据
}

ssize_t Serial::CANsend(const void *buf, size_t count) {
    std::lock_guard<std::mutex> lock(m_can_send_mutex);  // 自动加锁
    return write(CANfd, buf, count); // 写入数据
}

ssize_t Serial::CANsendTest() {
    const char msg[13] = {0x08, 0x00, 0x00, 0x00, 0xff, 0x01, 0x02, 0x03, 0x040, 0x05, 0x06, 0x07, 0x08}; // 写入数据
    return CANsend(msg, sizeof(msg));
}

int Serial::MCUopen() {
    const char *portname = "/dev/ttyS7"; // 替换为你自己的串口设备
    MCUfd = open(portname, O_RDWR | O_NOCTTY | O_SYNC);
    if (MCUfd < 0) {
        perror("open m_serial port");
        return 1;
    }

    if (MCUsetAttribs(B115200) < 0) { // 设置波特率为115200
        fprintf(stderr, "Error setting attributes\n");
        close(MCUfd);
        return 1;
    }
    return 0;
}

int Serial::MCUclose() {
    close(MCUfd);
    return 0;
}

int Serial::MCUsetAttribs(int speed) {
    struct termios tty;

    if (tcgetattr(MCUfd, &tty) != 0) {
        perror("tcgetattr");
        return -1;
    }

    cfsetospeed(&tty, speed);
    cfsetispeed(&tty, speed);

    tty.c_cflag |= (CLOCAL | CREAD); // 启用接收器
    tty.c_cflag &= ~PARENB;          // 无校验
    tty.c_cflag &= ~CSTOPB;          // 1个停止位
    tty.c_cflag &= ~CSIZE;           // 清除数据位掩码
    tty.c_cflag |= CS8;              // 8个数据位
    tty.c_cflag &= ~CRTSCTS;         // 无硬件流控制

    tty.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); // 非规范模式
    tty.c_oflag &= ~OPOST;                          // 原始输出

    tty.c_cc[VMIN] = 1;   // 读取至少1个字符
    tty.c_cc[VTIME] = 10; // 等待时间（单位为 0.1 秒）

    if (tcsetattr(MCUfd, TCSANOW, &tty) != 0) {
        perror("tcsetattr");
        return -1;
    }

    return 0;
}

ssize_t Serial::MCUreceive(void *buf, size_t count) {
    return read(MCUfd, buf, count); // 读取数据
}

ssize_t Serial::MCUsend(const void *buf, size_t count) {
    std::lock_guard<std::mutex> lock(m_mcu_send_mutex);  // 自动加锁
    return write(MCUfd, buf, count); // 写入数据
}

ssize_t Serial::MCUsendTest() {
    const char msg[13] = {0x02, 0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 0xff, 0x00, 0x01, 0xff, 0x01, 0x02}; // 写入数据
    return MCUsend(msg, sizeof(msg));
}
