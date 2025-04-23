#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <unistd.h>
#include <sys/wait.h>

void print_stack_limit() {
    struct rlimit limit;
    if (getrlimit(RLIMIT_STACK, &limit) == 0) {
        printf("Current stack size limit: soft = %ld KB, hard = %ld KB\n",
               limit.rlim_cur / 1024, limit.rlim_max / 1024);
    } else {
        perror("getrlimit");
    }
}

int main() {
    // 打印当前栈大小限制
    printf("Initial stack size:\n");
    print_stack_limit();

    // 修改栈大小限制为 8 MB
    struct rlimit new_limit;
    new_limit.rlim_cur = 16 * 1024 * 1024; // 软限制：8 MB
    new_limit.rlim_max = 16 * 1024 * 1024; // 硬限制：8 MB

    if (setrlimit(RLIMIT_STACK, &new_limit) != 0) {
        perror("setrlimit");
        exit(EXIT_FAILURE);
    }

    printf("\nStack size after modification:\n");
    print_stack_limit();

    // 创建子进程，验证作用域
    pid_t pid = fork();
    if (pid == 0) {
        // 子进程
        printf("\nChild process stack size:\n");
        print_stack_limit();
        exit(EXIT_SUCCESS);
    } else if (pid > 0) {
        // 父进程
        wait(NULL); // 等待子进程结束
        printf("\nParent process stack size after child exits:\n");
        print_stack_limit();
    } else {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    return 0;
}