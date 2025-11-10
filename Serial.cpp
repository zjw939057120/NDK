//
// Created by Administrator on 2025/6/9.
//

#include "Serial.h"
#include "ToolKits.h"

int Serial::Serial_open(const char *path, int &fd, int speed) {
    fd = open(path, O_RDWR | O_NOCTTY | O_SYNC);
    if (fd < 0) {
        perror("open m_serial port");
        return 1;
    }

    if (Serial_setAttribs(fd, speed) < 0) { // 设置波特率为115200
        fprintf(stderr, "Error setting attributes\n");
        close(fd);
        return 1;
    }
    return 0;
}

int Serial::Serial_close(int &fd) {
    close(fd);
    return 0;
}

int Serial::Serial_setAttribs(int &fd, int speed) {
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

ssize_t Serial::Serial_receive(int &fd, void *buf, size_t count) {
    return read(fd, buf, count); // 读取数据
}

ssize_t Serial::Serial_send(int &fd, const void *buf, size_t count) {
    return write(fd, buf, count); // 写入数据
}

void Serial::Serial_init() {
    //打开CAN模块
    if (ToolKits::isFileExists(UART0_PATH))
        Serial0_open();

    //打开控制器模块
    if (ToolKits::isFileExists(UART1_PATH))
        Serial1_open();

    //打开继电器模块
    if (ToolKits::isFileExists(UART2_PATH))
        Serial2_open();

    //打开惯导模块
    if (ToolKits::isFileExists(UART3_PATH))
        Serial3_open();
}


int Serial::Serial0_open() {
    return Serial_open(UART0_PATH, uart0_fd, UART0_SPEED);
}

int Serial::Serial0_close() {
    uart0_fd = Serial_close(uart0_fd);
    return uart0_fd;
}

ssize_t Serial::Serial0_receive(void *buf, size_t count) {
    return Serial_receive(uart0_fd, buf, count);
}

ssize_t Serial::Serial0_send(const void *buf, size_t count) {
    std::lock_guard<std::mutex> lock(uart0_mutex);  // 自动加锁
    return Serial_send(uart0_fd, buf, count);
}

ssize_t Serial::Serial0_sendTest() {
    const char msg[13] = {0x08, 0x00, 0x00, 0x00, 0xff, 0x01, 0x02, 0x03, 0x040, 0x05, 0x06, 0x07, 0x08}; // 写入数据
    return Serial0_send(msg, sizeof(msg));
}

int Serial::Serial1_open() {
    return Serial_open(UART1_PATH, uart1_fd, UART1_SPEED);
}

int Serial::Serial1_close() {
    uart1_fd = Serial_close(uart1_fd);
    return uart1_fd;
}

ssize_t Serial::Serial1_receive(void *buf, size_t count) {
    return Serial_receive(uart1_fd, buf, count);
}

ssize_t Serial::Serial1_send(const void *buf, size_t count) {
    std::lock_guard<std::mutex> lock(uart1_mutex);  // 自动加锁
    return Serial_send(uart1_fd, buf, count);
}

ssize_t Serial::Serial1_sendTest() {
    const char msg[13] = {0x08, 0x00, 0x00, 0x00, 0xff, 0x01, 0x02, 0x03, 0x040, 0x05, 0x06, 0x07, 0x08}; // 写入数据
    return Serial1_send(msg, sizeof(msg));
}

int Serial::Serial2_open() {
    return Serial_open(UART2_PATH, uart2_fd, UART2_SPEED);
}

int Serial::Serial2_close() {
    uart2_fd = Serial_close(uart2_fd);
    return uart2_fd;
}

ssize_t Serial::Serial2_receive(void *buf, size_t count) {
    return Serial_receive(uart2_fd, buf, count);
}

ssize_t Serial::Serial2_send(const void *buf, size_t count) {
    std::lock_guard<std::mutex> lock(uart2_mutex);  // 自动加锁
    return Serial_send(uart2_fd, buf, count);
}

ssize_t Serial::Serial2_sendTest() {
    const char msg[13] = {0x08, 0x00, 0x00, 0x00, 0xff, 0x01, 0x02, 0x03, 0x040, 0x05, 0x06, 0x07, 0x08}; // 写入数据
    return Serial2_send(msg, sizeof(msg));
}

int Serial::Serial3_open() {
    return Serial_open(UART3_PATH, uart3_fd, UART3_SPEED);
}

int Serial::Serial3_close() {
    uart3_fd = Serial_close(uart3_fd);
    return uart3_fd;
}

ssize_t Serial::Serial3_receive(void *buf, size_t count) {
    return Serial_receive(uart3_fd, buf, count);
}

ssize_t Serial::Serial3_send(const void *buf, size_t count) {
    std::lock_guard<std::mutex> lock(uart3_mutex);  // 自动加锁
    return Serial_send(uart3_fd, buf, count);
}

ssize_t Serial::Serial3_sendTest() {
    const char msg[13] = {0x08, 0x00, 0x00, 0x00, 0xff, 0x01, 0x02, 0x03, 0x040, 0x05, 0x06, 0x07, 0x08}; // 写入数据
    return Serial3_send(msg, sizeof(msg));
}


int Serial::Serial4_open() {
    return Serial_open(UART4_PATH, uart4_fd, UART4_SPEED);
}

int Serial::Serial4_close() {
    uart4_fd = Serial_close(uart4_fd);
    return uart4_fd;
}

ssize_t Serial::Serial4_receive(void *buf, size_t count) {
    return Serial_receive(uart4_fd, buf, count);
}

ssize_t Serial::Serial4_send(const void *buf, size_t count) {
    std::lock_guard<std::mutex> lock(uart4_mutex);  // 自动加锁
    return Serial_send(uart4_fd, buf, count);
}

ssize_t Serial::Serial4_sendTest() {
    const char msg[13] = {0x08, 0x00, 0x00, 0x00, 0xff, 0x01, 0x02, 0x03, 0x040, 0x05, 0x06, 0x07, 0x08}; // 写入数据
    return Serial4_send(msg, sizeof(msg));
}

int Serial::Serial5_open() {
    return Serial_open(UART5_PATH, uart5_fd, UART5_SPEED);
}

int Serial::Serial5_close() {
    uart5_fd = Serial_close(uart5_fd);
    return uart5_fd;
}

ssize_t Serial::Serial5_receive(void *buf, size_t count) {
    return Serial_receive(uart5_fd, buf, count);
}

ssize_t Serial::Serial5_send(const void *buf, size_t count) {
    std::lock_guard<std::mutex> lock(uart5_mutex);  // 自动加锁
    return Serial_send(uart5_fd, buf, count);
}

ssize_t Serial::Serial5_sendTest() {
    const char msg[13] = {0x08, 0x00, 0x00, 0x00, 0xff, 0x01, 0x02, 0x03, 0x040, 0x05, 0x06, 0x07, 0x08}; // 写入数据
    return Serial5_send(msg, sizeof(msg));
}


int Serial::Serial6_open() {
    return Serial_open(UART6_PATH, uart6_fd, UART6_SPEED);
}

int Serial::Serial6_close() {
    uart6_fd = Serial_close(uart6_fd);
    return uart6_fd;
}

ssize_t Serial::Serial6_receive(void *buf, size_t count) {
    return Serial_receive(uart6_fd, buf, count);
}

ssize_t Serial::Serial6_send(const void *buf, size_t count) {
    std::lock_guard<std::mutex> lock(uart6_mutex);  // 自动加锁
    return Serial_send(uart6_fd, buf, count);
}

ssize_t Serial::Serial6_sendTest() {
    const char msg[13] = {0x08, 0x00, 0x00, 0x00, 0xff, 0x01, 0x02, 0x03, 0x040, 0x05, 0x06, 0x07, 0x08}; // 写入数据
    return Serial3_send(msg, sizeof(msg));
}

int Serial::Serial7_open() {
    return Serial_open(UART7_PATH, uart7_fd, UART7_SPEED);
}

int Serial::Serial7_close() {
    uart7_fd = Serial_close(uart7_fd);
    return uart7_fd;
}

ssize_t Serial::Serial7_receive(void *buf, size_t count) {
    return Serial_receive(uart7_fd, buf, count);
}

ssize_t Serial::Serial7_send(const void *buf, size_t count) {
    std::lock_guard<std::mutex> lock(uart7_mutex);  // 自动加锁
    return Serial_send(uart7_fd, buf, count);
}

ssize_t Serial::Serial7_sendTest() {
    const char msg[13] = {0x08, 0x00, 0x00, 0x00, 0xff, 0x01, 0x02, 0x03, 0x040, 0x05, 0x06, 0x07, 0x08}; // 写入数据
    return Serial7_send(msg, sizeof(msg));
}
