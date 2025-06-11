//
// Created by Administrator on 2025/6/10.
//

#ifndef TCPSERVER_TOOLKITS_H
#define TCPSERVER_TOOLKITS_H


#include <cstdint>
#include <string>

#define SWITCH_1 32 //GPIO0_A0
#define SWITCH_2 33 //GPIO0_A1
#define SWITCH_3 36 //GPIO0_A4
#define SWITCH_4 40 //GPIO1_B0
#define SWITCH_5 41 //GPIO1_B1
#define SWITCH_6 42 //GPIO1_B2

class ToolKits {
public:
    static void EnvInit();

    static void GPIOInit(uint8_t GPIO);

    static void GPIOSetHigh(uint8_t GPIO);

    static void GPIOSetLow(uint8_t GPIO);

    static uint8_t GPIOGetValue(uint8_t GPIO);

    static std::string execCommand(const char *cmd);

    static std::string getSerial();

    static void dump(const uint8_t *data, size_t len);
};


#endif //TCPSERVER_TOOLKITS_H
