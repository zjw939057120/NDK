//
// Created by Administrator on 2025/6/10.
//

#ifndef TCPSERVER_TOOLKITS_H
#define TCPSERVER_TOOLKITS_H


#include <cstdint>
#include <string>
#include <unistd.h>

#define SWITCH_1 32 //GPIO0_A0
#define SWITCH_2 33 //GPIO0_A1
#define SWITCH_3 36 //GPIO0_A4
#define SWITCH_4 40 //GPIO1_B0
#define SWITCH_5 41 //GPIO1_B1
#define SWITCH_6 42 //GPIO1_B2

//主板序列号
#define BOARD_SERIAL " /data/local/addition/tmp/serial"
//主板网关
#define BOARD_GATEWAY " /data/local/addition/tmp/gateway"

class ToolKits {
public:

    static void GPIOInit(uint8_t GPIO);

    static void GPIOSetHigh(uint8_t GPIO);

    static void GPIOSetLow(uint8_t GPIO);

    static uint8_t GPIOGetValue(uint8_t GPIO);

    static std::string execCommand(const char *cmd);

    static std::string getSerialNumber();

    static void dump(const uint8_t *data, size_t len);

    static bool isFileExists(const std::string& filename);

    static bool isDeviceExist(const std::string& devicePath);

    static void disablePackage(const std::string& package);

    static std::string getGateway() ;
};


#endif //TCPSERVER_TOOLKITS_H
