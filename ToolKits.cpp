//
// Created by Administrator on 2025/6/10.
//

#include <cstdlib>
#include "ToolKits.h"

void ToolKits::EnvInit() {
    getSerial();
    GPIOInit(SWITCH_1);
    GPIOInit(SWITCH_2);
    GPIOInit(SWITCH_3);
    GPIOInit(SWITCH_4);
    GPIOInit(SWITCH_5);
    GPIOInit(SWITCH_6);
}

void ToolKits::GPIOInit(uint8_t GPIO) {
    char command[255];
    sprintf(command, "echo %d > /sys/class/gpio/export", GPIO);
    system(command);
    sprintf(command, "echo out > /sys/class/gpio/gpio%d/direction", GPIO);
    system(command);
    GPIOSetLow(GPIO);
}

void ToolKits::GPIOSetHigh(uint8_t GPIO) {
    char command[255];
    sprintf(command, "echo 1 > /sys/class/gpio/gpio%d/value", GPIO);
    system(command);
}

void ToolKits::GPIOSetLow(uint8_t GPIO) {
    char command[255];
    sprintf(command, "echo 0 > /sys/class/gpio/gpio%d/value", GPIO);
    system(command);
}

uint8_t ToolKits::GPIOGetValue(uint8_t GPIO) {
    char command[255];
    sprintf(command, "cat /sys/class/gpio/gpio%d/value | tr -d '\\n'", GPIO);
    std::string ret = execCommand(command);
    return atoi(ret.c_str());
}

std::string ToolKits::execCommand(const char *cmd) {
    char buffer[512];
    FILE *pipe = popen(cmd, "r"); // 将"ls -l"替换为你想要执行的命令

    if (!pipe) {
        perror("popen");
        return "";
    }

    while (fgets(buffer, sizeof(buffer), pipe) != NULL) {
        //printf("%s", buffer);
    }

    if (pclose(pipe) == -1) {
        perror("pclose");
        return "";
    }

    return buffer;
}

std::string ToolKits::getSerial() {
    char command[255];
    sprintf(command, "grep \"Serial\" /proc/cpuinfo  | cut -d':' -f2 | xargs | tr -d '\\n'");
    std::string ret = execCommand(command);
    sprintf(command, "echo -n %s > /data/local/addition/Serial", ret.c_str());
    system(command);
    system("chmod 755 /data/local/addition/Serial");
    return ret;
}

void ToolKits::dump(const uint8_t *data, size_t len) {
    for (int i = 0; i < len; ++i) {
        printf("%02x ", data[i]);
    }
    printf("\n");
}

bool ToolKits::is_file_exists(const std::string &filename) {
    return access(filename.c_str(), F_OK) != -1;
}
