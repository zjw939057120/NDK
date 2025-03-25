#include <iostream>
#include <unistd.h>
#include "socket.h"

int main() {
    std::cout << "welcome jt905 client" << std::endl;

    pthread_t thread_id;
    // 创建新线程
    pthread_create(&thread_id, NULL, reinterpret_cast<void *(*)(void *)>(socket_pthread), NULL);
    // 等待新线程结束
    while (1) {
        sleep(30);
    }
    return 0;
}
