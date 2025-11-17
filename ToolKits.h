//
// Created by Administrator on 2025/6/10.
//

#ifndef TCPSERVER_TOOLKITS_H
#define TCPSERVER_TOOLKITS_H


#include <cstdint>
#include <string>
#include <unistd.h>

//主板序列号
#define BOARD_SERIAL " /data/local/addition/tmp/serial"
//主板网关
#define BOARD_GATEWAY " /data/local/addition/tmp/gateway"
//触屏事件节点
#define INPUT_EVENT_TOUCH_PATH " /dev/input/event5"
//继电器0号打开标识
#define RELAY_ON_0_LOCK "/data/local/addition/tmp/relay_on_0.lock"
//继电器1号打开标识
#define RELAY_ON_1_LOCK "/data/local/addition/tmp/relay_on_1.lock"
//继电器2号打开标识
#define RELAY_ON_2_LOCK " /data/local/addition/tmp/relay_on_2.lock"
//继电器3号打开标识
#define RELAY_ON_3_LOCK " /data/local/addition/tmp/relay_on_3.lock"
//继电器3号打开标识
//初始化完成标识
#define TCP_SERVER_LOCK "/data/local/addition/TcpServer.lock"
//日志文件路径
#define TCP_SERVER_LOG "/data/local/addition/TcpServer.log"
//DEMO模式标识
#define DEMO_LOCK "/data/local/addition/demo.lock"
//中控屏app包名
#define PACKAGE_COM_HBTENGLV_BOAT "com.hbtenglv.boat"
//娱乐屏app包名
#define PACKAGE_COM_HBTENGLV_BOAT_HOME "com.hbtenglv.boathome"

class ToolKits {
public:

    static void GPIOInit(uint8_t GPIO);

    static void GPIOSetHigh(uint8_t GPIO);

    static void GPIOSetLow(uint8_t GPIO);

    static uint8_t GPIOGetValue(uint8_t GPIO);

    static std::string execCommand(const char *cmd);

    static std::string getSerialNumber();

    static void dump(const uint8_t *data, size_t len);

    static bool isFileExists(const std::string &filename);

    static bool isDeviceExist(const std::string &devicePath);

    static void disablePackage(const std::string &package);

    static void enablePackage(const std::string &package);

    static std::string getGateway();

    static void createFile(const char file[]);

    static void deleteFile(const char *file);

    static void systemOpenSettings();

    static void systemPowerShutown();

    static void systemPowerReboot();

    static void systemPowerRebootLoader();

    static void systemKeyCodeHome();

    static void systemKeyCodeMenu();

    static void systemKeyCodeVolueDown();

    static void systemKeyCodeVolueUp();

    static void systemKeyCodeVolueMute();

    static void systemKeyCodePower();

    static void systemKeyCodeSleep();

    static void systemKeyCodeWakeup();

    static void systemScreenOff();

    static void systemScreenOn();
};

#endif //TCPSERVER_TOOLKITS_H
