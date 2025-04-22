#include <iostream>
#include <csignal>
#include <cstdlib>
#include <unwind.h> // libunwind

// 定义一个结构体用于存储调用栈信息
struct BacktraceState {
    void **current;
    void **end;
};

// 回调函数，用于遍历调用栈
static _Unwind_Reason_Code unwindCallback(struct _Unwind_Context *context, void *arg) {
    BacktraceState *state = static_cast<BacktraceState *>(arg);
    uintptr_t pc = _Unwind_GetIP(context);
    if (pc) {
        if (state->current == state->end) {
            return _URC_END_OF_STACK;
        } else {
            *state->current++ = reinterpret_cast<void *>(pc);
        }
    }
    return _URC_NO_REASON;
}

// 获取调用栈
size_t captureBacktrace(void **buffer, size_t max) {
    BacktraceState state = {buffer, buffer + max};
    _Unwind_Backtrace(unwindCallback, &state);
    return state.current - buffer;
}

// 打印调用栈
void dumpBacktrace(std::ostream &os, void **buffer, size_t count) {
    for (size_t idx = 0; idx < count; ++idx) {
        os << "  #" << idx << "  " << buffer[idx] << "\n";
    }
}

// 信号处理函数
void signalHandler(int signal) {
    if (signal == SIGSEGV) {
        const size_t max = 32;
        void *buffer[max];
        size_t count = captureBacktrace(buffer, max);

        std::cerr << "Backtrace of the error:\n";
        dumpBacktrace(std::cerr, buffer, count);

        std::exit(EXIT_FAILURE);
    }
}

int main() {
    // 设置信号处理器
    std::signal(SIGSEGV, signalHandler);
    int *ptr = (int *)0x12345678; // 假设一个无效的内存地址

    printf("Attempting to access invalid memory...\n");

    *ptr = 42; // 尝试写入无效地址

    return 0;
}