#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fstream>
#include <iostream>

#define KEY1 108
#define KEY2 107
#define KEY3 105

void broadcast(int key) {
    char cmd[64];
    sprintf(cmd, "/system/bin/am broadcast -a com.xstrive.qdcar --es key %d", key);
    system(cmd);
}

void export_gpio(int gpio_num) {
    char cmd[64];
    sprintf(cmd, "echo %d > /sys/class/gpio/export", gpio_num);
    system(cmd);
}

void set_gpio_input(int gpio_num) {
    char cmd[64];
    sprintf(cmd, "echo %d > /sys/class/gpio/export", gpio_num);
    system(cmd);
    sprintf(cmd, "echo in > /sys/class/gpio/gpio%d/direction", gpio_num);
    system(cmd);
}

int read_gpio_value(int gpio_num) {
    char cmd[50];
    sprintf(cmd, "/sys/class/gpio/gpio%d/value", gpio_num);
    // 以只读方式打开文件
    std::ifstream file(cmd);

    // 检查文件是否成功打开
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << cmd << std::endl;
        return -1;
    }

    int value;
    // 逐行读取文件内容
    std::string line;
    while (std::getline(file, line)) {
        value = stoi(line);
    }
    // 关闭文件
    file.close();

    if (value == 0) {
        broadcast(gpio_num);
    }
    return value;
}

int main() {
    // 导出 GPIO
    export_gpio(KEY1);
    export_gpio(KEY2);
    export_gpio(KEY3);


    // 设置 GPIO 为输入
    set_gpio_input(KEY1);
    set_gpio_input(KEY2);
    set_gpio_input(KEY3);


    while (1) {
        // 读取 GPIO 状态
        read_gpio_value(KEY1);
        read_gpio_value(KEY2);
        read_gpio_value(KEY3);
        usleep(200000);
    }

    return 0;
}
