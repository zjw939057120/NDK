//
// Created by Administrator on 2025/6/10.
//

#include <cstdlib>
#include "ToolKits.h"
#include <sys/stat.h>  // for stat()

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
    std::string result;
    FILE *pipe = popen(cmd, "r"); // 将"ls -l"替换为你想要执行的命令

    if (!pipe) {
        perror("popen");
        return "";
    }

    while (fgets(buffer, sizeof(buffer), pipe) != NULL) {
        result += buffer;
        //printf("%s", buffer);
    }

    if (pclose(pipe) == -1) {
        perror("pclose");
        return "";
    }

    return result;
}

std::string ToolKits::getSerialNumber() {
    char command[255];
    sprintf(command, "grep \"Serial\" /proc/cpuinfo  | cut -d':' -f2 | xargs | tr -d '\\n'");
    std::string ret = execCommand(command);
    printf("the Serial is %s\r\n", ret.c_str());
    sprintf(command, "echo -n %s > %s", ret.c_str(), BOARD_SERIAL);
    system(command);
    sprintf(command, "chmod 755 %s", BOARD_SERIAL);
    system(command);
    system("id > /data/local/addition/id");
    return ret;
}

void ToolKits::dump(const uint8_t *data, size_t len) {
    for (int i = 0; i < len; ++i) {
        printf("%02x ", data[i]);
    }
    printf("\n");
}

bool ToolKits::isFileExists(const std::string &filename) {
    return access(filename.c_str(), F_OK) != -1;
}

bool ToolKits::isDeviceExist(const std::string &devicePath) {
    struct stat buffer;
    if (stat(devicePath.c_str(), &buffer) == 0) {
        // 文件存在，可以进一步检查是否为字符设备
        if (S_ISCHR(buffer.st_mode))
            return true;
        else
            return false;
    } else {
        return false;
    }
}

void ToolKits::disablePackage(const std::string &package) {
    char command[255];
    sprintf(command, "pm disable-user --user 0 %s", package.c_str());
    system(command);
}


std::string ToolKits::getGateway() {
    char command[255];
    sprintf(command, "ip route show table eth0 | head -n1 | awk '{print $3}' | tr -d '\\n'");
    std::string ret = execCommand(command);
    if (!ret.empty()) {
        sprintf(command, "echo -n %s > %s", ret.c_str(), BOARD_GATEWAY);
        system(command);
        sprintf(command, "chmod 755 %s", BOARD_GATEWAY);
        system(command);
    }
    return ret;
}

