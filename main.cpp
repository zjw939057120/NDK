#include <iostream>
#include <unistd.h>
#include "socket.h"
#include "client.h"

int main() {
    std::cout << "welcome jt905 client" << std::endl;


    // 创建新线程
    pthread_t thread1;
    pthread_create(&thread1, NULL, reinterpret_cast<void *(*)(void *)>(socket_pthread), NULL);
    // 创建新线程
    pthread_t thread2;
    pthread_create(&thread2, NULL, reinterpret_cast<void *(*)(void *)>(jt1078_pthread), NULL);
    while (1) {
        sleep(30);
    }
    return 0;
}
