#include "main.h"
#include "Daemon.h"

int UART_TcpServer_Instance(TcpServer &srv, int port, const std::function<void(Buffer *)> &onMessage) {
    int listenfd = srv.createsocket(port);
    if (listenfd < 0) {
        return -20;
    }
    printf("server listen on port %d, listenfd=%d ...\n", port, listenfd);
    srv.onConnection = [](const SocketChannelPtr &channel) {
        std::string peeraddr = channel->peeraddr();
        if (channel->isConnected()) {
            printf("%s connected! connfd=%d id=%d tid=%ld\n", peeraddr.c_str(), channel->fd(), channel->id(),
                   currentThreadEventLoop->tid());
        } else {
            printf("%s disconnected! connfd=%d id=%d tid=%ld\n", peeraddr.c_str(), channel->fd(), channel->id(),
                   currentThreadEventLoop->tid());
        }
    };
    srv.onMessage = [onMessage](const SocketChannelPtr &channel, Buffer *buf) {
        onMessage(buf);
    };
    srv.setThreadNum(4);
    srv.setLoadBalance(LB_LeastConnections);

#if TEST_TLS
    hssl_ctx_opt_t ssl_opt;
memset(&ssl_opt, 0, sizeof(hssl_ctx_opt_t));
ssl_opt.crt_file = "cert/server.crt";
ssl_opt.key_file = "cert/server.key";
ssl_opt.verify_peer = 0;
srv.withTLS(&ssl_opt);
#endif

    srv.start();
    return 0;
};

int TCP_Client_Instance(TcpClient &cli, int remote_port, const char *remote_host,
                        const std::function<void(Buffer *)> &onMessage) {
    int connfd = cli.createsocket(remote_port, remote_host);
    if (connfd < 0) {
        return -20;
    }
    printf("client connect to port %d, connfd=%d ...\n", remote_port, connfd);
    cli.onConnection = [&cli](const SocketChannelPtr &channel) {
        std::string peeraddr = channel->peeraddr();
        if (channel->isConnected()) {
            printf("connected to %s! connfd=%d\n", peeraddr.c_str(), channel->fd());
        } else {
            printf("disconnected to %s! connfd=%d\n", peeraddr.c_str(), channel->fd());
        }
        if (cli.isReconnect()) {
            printf("reconnect cnt=%d, delay=%d\n", cli.reconn_setting->cur_retry_cnt, cli.reconn_setting->cur_delay);
        }
    };

    cli.onMessage = [onMessage](const SocketChannelPtr &channel, Buffer *buf) {
        onMessage(buf);
    };

#if TEST_RECONNECT
    // reconnect: 1,2,4,8,10,10,10...
    reconn_setting_t reconn;
    reconn_setting_init(&reconn);
    reconn.min_delay = 1000;
    reconn.max_delay = 10000;
    reconn.delay_policy = 2;
    cli.setReconnect(&reconn);
#endif

#if TEST_TLS
    cli.withTLS();
#endif

    cli.start();
    return 0;
}

void onceInit() {
    if (ToolKits::isFileExists(TCP_SERVER_LOCK))
        return;

    //取消通知提示音
    std::system("settings put system notification_sound null");
    //存在串口模块判定为中控系统
    if (ToolKits::isDeviceExist(EC20_PATH)) {
        printf("the %s found\r\n", EC20_PATH);
        ToolKits::enablePackage(PACKAGE_COM_HBTENGLV_BOAT);
        //禁用娱乐屏app
        ToolKits::disablePackage(PACKAGE_COM_HBTENGLV_BOAT_HOME);
    } else {
        ToolKits::enablePackage(PACKAGE_COM_HBTENGLV_BOAT_HOME);
        //禁用中控屏app
        ToolKits::disablePackage(PACKAGE_COM_HBTENGLV_BOAT);
    }

    FILE *fp = fopen(TCP_SERVER_LOCK, "w");
    fclose(fp);
}

int main(int argc, char *argv[]) {
    hlog_set_level(LOG_LEVEL_DEBUG);
    // 将 stdout 重定向到文件
    /*if (freopen(TCP_SERVER_LOG, "w", stdout) == nullptr) {
        perror("freopen stdout failed");
    }*/
    //记录主板唯一ID
    ToolKits::getSerialNumber();

    //环境初始化
    onceInit();
    //中控屏服务端应用
    if (ToolKits::isDeviceExist(EC20_PATH)) {
        Serial serial;
        TcpServer svr_sys;
        TcpServer srv_0;
        TcpServer srv_1;
        TcpServer srv_2;
        TcpServer srv_3;
        //串口初始化
        serial.Serial_init();

        //实例化服务端消息处理
        SerialMessageCallback serialMessageCallback(&serial, &svr_sys, &srv_0, &srv_1, &srv_2, &srv_3);

        //系统服务
        UART_TcpServer_Instance(svr_sys, SYS_PORT,
                                std::bind(&SerialMessageCallback::svr_sys_onMessageCallback, &serialMessageCallback,
                                          std::placeholders::_1));

        //串口0透传服务
        UART_TcpServer_Instance(srv_0, UART0_PORT,
                                std::bind(&SerialMessageCallback::svr0_onMessageCallback, &serialMessageCallback,
                                          std::placeholders::_1));

        //串口1透传服务
        UART_TcpServer_Instance(srv_1, UART1_PORT,
                                std::bind(&SerialMessageCallback::svr1_onMessageCallback, &serialMessageCallback,
                                          std::placeholders::_1));

        //串口2透传服务
        UART_TcpServer_Instance(srv_2, UART2_PORT,
                                std::bind(&SerialMessageCallback::svr2_onMessageCallback, &serialMessageCallback,
                                          std::placeholders::_1));

        //串口3透传服务
        UART_TcpServer_Instance(srv_3, UART3_PORT,
                                std::bind(&SerialMessageCallback::svr3_onMessageCallback, &serialMessageCallback,
                                          std::placeholders::_1));

        if (ToolKits::isFileExists(DEMO_LOCK)) {
            serialMessageCallback.svr_demoThread();
        }

        //实例化串口消息处理
        SerialReceiveHandle serialReceiveHandle(&serial, &srv_0, &srv_1, &srv_2, &srv_3);
        //串口消息处理线程
        serialReceiveHandle.Serial_receiveHandle();

        //后台服务
        Daemon daemon;
        daemon.init();
        daemon.setSrv(&svr_sys);
        daemon.openTouchDev();
        //触屏事件线程
        daemon.touchEventThread();
        //屏幕检测线程
        daemon.screenCheckThread();

        while (true) {
            if (!ToolKits::isDeviceExist(UART0_PATH)) {
                serial.Serial0_close();
                serial.Serial1_close();
                serial.Serial2_close();
                serial.Serial3_close();
                serial.Serial4_close();
                serial.Serial5_close();
                serial.Serial6_close();
                serial.Serial7_close();
            }
            sleep(60);
        }


    } else {
        //娱乐屏客户端应用
        std::string ip = ToolKits::getGateway();
        while (ip.empty()) {
            sleep(5);
            ip = ToolKits::getGateway();
        }
        printf("the eth0 gateway is %s\r\n", ip.c_str());

        TcpClient cli;
        //实例化客户端消息处理
        SystemMessageCallback systemMessageCallback;
        TCP_Client_Instance(cli, SYS_PORT, ip.c_str(),
                            std::bind(&SystemMessageCallback::onMessageCallback, &systemMessageCallback,
                                      std::placeholders::_1));


        while (true) {
            sleep(300);
        }

    }
    return 0;
}

