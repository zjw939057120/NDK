cmake_minimum_required(VERSION 3.19)

# 设置目标系统
set(CMAKE_SYSTEM_NAME Linux)
# 设置目标系统的处理器架构
set(CMAKE_SYSTEM_PROCESSOR arm64)

set(ANDROID_NDK_HOME F:/AndroidSdk/ndk/22.1.7171670)

# 设置交叉编译工具链的路径
set(CMAKE_C_COMPILER ${ANDROID_NDK_HOME}/toolchains/llvm/prebuilt/windows-x86_64/bin/aarch64-linux-android22-clang.cmd)
set(CMAKE_CXX_COMPILER ${ANDROID_NDK_HOME}/toolchains/llvm/prebuilt/windows-x86_64/bin/aarch64-linux-android22-clang++.cmd)

# 设置查找库和头文件的路径
set(CMAKE_FIND_ROOT_PATH ${ANDROID_NDK_HOME}/toolchains/llvm/prebuilt/windows-x86_64/sysroot)

project(ndk_compile)

set(CMAKE_CXX_STANDARD 14)

include_directories(3rdparty/include)
include_directories(3rdparty/include/hv)
link_directories(3rdparty/lib)

link_libraries(
        -llog
        -lhv_static
        -lnanomsg
)

add_executable(jt905 main.cpp socket.cpp socket.h henxing.cpp henxing.h client.cpp client.h)