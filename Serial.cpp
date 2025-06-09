//
// Created by Administrator on 2025/6/9.
//

#include "Serial.h"

int Serial::Open() {
    const char *portname = "/dev/ttyS3"; // 替换为你自己的串口设备
    fd = open(portname, O_RDWR | O_NOCTTY | O_SYNC);
    if (fd < 0) {
        perror("open serial port");
        return 1;
    }

    if (set_interface_attribs(B115200) < 0) { // 设置波特率为115200
        fprintf(stderr, "Error setting attributes\n");
        close(fd);
        return 1;
    }

    const char msg[13] = {0x08, 0x00, 0x00, 0x00, 0xff, 0x01, 0x02, 0x03, 0x040, 0x05, 0x06, 0x07, 0x08};
    write(fd, msg, sizeof(msg)); // 写入数据
//
//    uint8_t buffer[13];
//    while (1) {
//        int n = read(fd, buffer, sizeof(buffer)); // 读取数据
//        if (n > 0) {
//            printf("%d\n", n);
//            for (int i = 0; i < 13; ++i) {
//                printf("%02x ", buffer[i]);
//            }
//            printf("\n");
//        } else {
//            printf("No data received or error.\n");
//        }
//    }

    return 0;
}


int Serial::Close() {
    close(fd);
    return 0;
}

int Serial::set_interface_attribs(int speed) {
    struct termios tty;

    if (tcgetattr(fd, &tty) != 0) {
        perror("tcgetattr");
        return -1;
    }

    cfsetospeed(&tty, speed);
    cfsetispeed(&tty, speed);

    tty.c_cflag |= (CLOCAL | CREAD);   // 启用接收器
    tty.c_cflag &= ~PARENB;            // 无校验
    tty.c_cflag &= ~CSTOPB;            // 1个停止位
    tty.c_cflag &= ~CSIZE;             // 清除数据位掩码
    tty.c_cflag |= CS8;                // 8个数据位
    tty.c_cflag &= ~CRTSCTS;           // 无硬件流控制

    tty.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); // 非规范模式
    tty.c_oflag &= ~OPOST;             // 原始输出

    tty.c_cc[VMIN] = 1;                // 读取至少1个字符
    tty.c_cc[VTIME] = 10;              // 等待时间（单位为 0.1 秒）

    if (tcsetattr(fd, TCSANOW, &tty) != 0) {
        perror("tcsetattr");
        return -1;
    }

    return 0;
}

int Serial::Receive(void *buf, size_t count) {
    return read(fd, buf, count); // 读取数据
}

int Serial::Send(const void *buf, size_t count) {
    return write(fd, buf, count); // 写入数据
}

int Serial::SendTest() {
    const char msg[13] = {0x08, 0x00, 0x00, 0x00, 0xff, 0x01, 0x02, 0x03, 0x040, 0x05, 0x06, 0x07, 0x08};// 写入数据
    return Send(msg, sizeof(msg));
}

