//
// Created by Administrator on 2025/6/9.
//

#include "Serial.h"

int Serial::UART_open(const char *path, int &fd, int speed) {
    fd = open(path, O_RDWR | O_NOCTTY | O_SYNC);
    if (fd < 0) {
        perror("open m_serial port");
        return 1;
    }

    if (UART_setAttribs(fd, speed) < 0) { // 设置波特率为115200
        fprintf(stderr, "Error setting attributes\n");
        close(fd);
        return 1;
    }
    return 0;
}

int Serial::UART_close(int &fd) {
    close(fd);
    return 0;
}

int Serial::UART_setAttribs(int &fd, int speed) {
    struct termios tty;

    if (tcgetattr(fd, &tty) != 0) {
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

    if (tcsetattr(fd, TCSANOW, &tty) != 0) {
        perror("tcsetattr");
        return -1;
    }

    return 0;
}

ssize_t Serial::UART_receive(int &fd, void *buf, size_t count) {
    return read(fd, buf, count); // 读取数据
}

ssize_t Serial::UART_send(int &fd, const void *buf, size_t count) {
    return write(fd, buf, count); // 写入数据
}

int Serial::UART0_open() {
    return UART_open(UART0_PATH, uart0_fd, UART0_SPEED);
}

int Serial::UART0_close() {
    return UART_close(uart0_fd);
}

ssize_t Serial::UART0_receive(void *buf, size_t count) {
    return UART_receive(uart0_fd, buf, count);
}

ssize_t Serial::UART0_send(const void *buf, size_t count) {
    std::lock_guard<std::mutex> lock(uart0_mutex);  // 自动加锁
    return UART_send(uart0_fd, buf, count);
}

ssize_t Serial::UART0_sendTest() {
    const char msg[13] = {0x08, 0x00, 0x00, 0x00, 0xff, 0x01, 0x02, 0x03, 0x040, 0x05, 0x06, 0x07, 0x08}; // 写入数据
    return UART0_send(msg, sizeof(msg));
}

int Serial::UART1_open() {
    return UART_open(UART1_PATH, uart1_fd, UART1_SPEED);
}

int Serial::UART1_close() {
    return UART_close(uart1_fd);
}

ssize_t Serial::UART1_receive(void *buf, size_t count) {
    return UART_receive(uart1_fd, buf, count);
}

ssize_t Serial::UART1_send(const void *buf, size_t count) {
    std::lock_guard<std::mutex> lock(uart1_mutex);  // 自动加锁
    return UART_send(uart1_fd, buf, count);
}

ssize_t Serial::UART1_sendTest() {
    const char msg[13] = {0x08, 0x00, 0x00, 0x00, 0xff, 0x01, 0x02, 0x03, 0x040, 0x05, 0x06, 0x07, 0x08}; // 写入数据
    return UART1_send(msg, sizeof(msg));
}

int Serial::UART2_open() {
    return UART_open(UART2_PATH, uart2_fd, UART2_SPEED);
}

int Serial::UART2_close() {
    return UART_close(uart2_fd);
}

ssize_t Serial::UART2_receive(void *buf, size_t count) {
    return UART_receive(uart2_fd, buf, count);
}

ssize_t Serial::UART2_send(const void *buf, size_t count) {
    std::lock_guard<std::mutex> lock(uart2_mutex);  // 自动加锁
    return UART_send(uart2_fd, buf, count);
}

ssize_t Serial::UART2_sendTest() {
    const char msg[13] = {0x08, 0x00, 0x00, 0x00, 0xff, 0x01, 0x02, 0x03, 0x040, 0x05, 0x06, 0x07, 0x08}; // 写入数据
    return UART2_send(msg, sizeof(msg));
}

int Serial::UART3_open() {
    return UART_open(UART3_PATH, uart3_fd, UART3_SPEED);
}

int Serial::UART3_close() {
    return UART_close(uart3_fd);
}

ssize_t Serial::UART3_receive(void *buf, size_t count) {
    return UART_receive(uart3_fd, buf, count);
}

ssize_t Serial::UART3_send(const void *buf, size_t count) {
    std::lock_guard<std::mutex> lock(uart3_mutex);  // 自动加锁
    return UART_send(uart3_fd, buf, count);
}

ssize_t Serial::UART3_sendTest() {
    const char msg[13] = {0x08, 0x00, 0x00, 0x00, 0xff, 0x01, 0x02, 0x03, 0x040, 0x05, 0x06, 0x07, 0x08}; // 写入数据
    return UART3_send(msg, sizeof(msg));
}

#ifdef UART_8CHN

int Serial::UART4_open() {
    return UART_open(UART4_PATH, uart4_fd, UART4_SPEED);
}

int Serial::UART4_close() {
    return UART_close(uart4_fd);
}

ssize_t Serial::UART4_receive(void *buf, size_t count) {
    return UART_receive(uart4_fd, buf, count);
}

ssize_t Serial::UART4_send(const void *buf, size_t count) {
    std::lock_guard<std::mutex> lock(uart4_mutex);  // 自动加锁
    return UART_send(uart4_fd, buf, count);
}

ssize_t Serial::UART4_sendTest() {
    const char msg[13] = {0x08, 0x00, 0x00, 0x00, 0xff, 0x01, 0x02, 0x03, 0x040, 0x05, 0x06, 0x07, 0x08}; // 写入数据
    return UART4_send(msg, sizeof(msg));
}

int Serial::UART5_open() {
    return UART_open(UART5_PATH, uart5_fd, UART5_SPEED);
}

int Serial::UART5_close() {
    return UART_close(uart5_fd);
}

ssize_t Serial::UART5_receive(void *buf, size_t count) {
    return UART_receive(uart5_fd, buf, count);
}

ssize_t Serial::UART5_send(const void *buf, size_t count) {
    std::lock_guard<std::mutex> lock(uart5_mutex);  // 自动加锁
    return UART_send(uart5_fd, buf, count);
}

ssize_t Serial::UART5_sendTest() {
    const char msg[13] = {0x08, 0x00, 0x00, 0x00, 0xff, 0x01, 0x02, 0x03, 0x040, 0x05, 0x06, 0x07, 0x08}; // 写入数据
    return UART5_send(msg, sizeof(msg));
}


int Serial::UART6_open() {
    return UART_open(UART6_PATH, uart6_fd, UART6_SPEED);
}

int Serial::UART6_close() {
    return UART_close(uart6_fd);
}

ssize_t Serial::UART6_receive(void *buf, size_t count) {
    return UART_receive(uart6_fd, buf, count);
}

ssize_t Serial::UART6_send(const void *buf, size_t count) {
    std::lock_guard<std::mutex> lock(uart6_mutex);  // 自动加锁
    return UART_send(uart6_fd, buf, count);
}

ssize_t Serial::UART6_sendTest() {
    const char msg[13] = {0x08, 0x00, 0x00, 0x00, 0xff, 0x01, 0x02, 0x03, 0x040, 0x05, 0x06, 0x07, 0x08}; // 写入数据
    return UART3_send(msg, sizeof(msg));
}

int Serial::UART7_open() {
    return UART_open(UART7_PATH, uart7_fd, UART7_SPEED);
}

int Serial::UART7_close() {
    return UART_close(uart7_fd);
}

ssize_t Serial::UART7_receive(void *buf, size_t count) {
    return UART_receive(uart7_fd, buf, count);
}

ssize_t Serial::UART7_send(const void *buf, size_t count) {
    std::lock_guard<std::mutex> lock(uart7_mutex);  // 自动加锁
    return UART_send(uart7_fd, buf, count);
}

ssize_t Serial::UART7_sendTest() {
    const char msg[13] = {0x08, 0x00, 0x00, 0x00, 0xff, 0x01, 0x02, 0x03, 0x040, 0x05, 0x06, 0x07, 0x08}; // 写入数据
    return UART7_send(msg, sizeof(msg));
}

#endif