//
// Created by Administrator on 2025/6/9.
//

#ifndef TCPSERVER_SERIAL_H
#define TCPSERVER_SERIAL_H

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <termios.h>

class Serial {

public:
    int Open();

    int Close();

    int set_interface_attribs(int speed);

    int Receive(void *buf, size_t count);

    int Send(const void *buf, size_t count);

    int SendTest();

private:
    int fd = 0;
};


#endif //TCPSERVER_SERIAL_H
