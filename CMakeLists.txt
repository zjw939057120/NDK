# cmake_minimum_required(VERSION <specify CMake version here>)
cmake_minimum_required(VERSION 3.19)
# 设置目标系统
set(CMAKE_SYSTEM_NAME Linux)
# 设置目标系统的处理器架构
set(CMAKE_SYSTEM_PROCESSOR arm64)

set(ANDROID_NDK_HOME D:/AndroidSdk/ndk/25.1.8937393)

# 设置交叉编译工具链的路径
set(CMAKE_C_COMPILER ${ANDROID_NDK_HOME}/toolchains/llvm/prebuilt/windows-x86_64/bin/aarch64-linux-android27-clang.cmd)
set(CMAKE_CXX_COMPILER ${ANDROID_NDK_HOME}/toolchains/llvm/prebuilt/windows-x86_64/bin/aarch64-linux-android27-clang++.cmd)

# 设置查找库和头文件的路径
set(CMAKE_FIND_ROOT_PATH ${ANDROID_NDK_HOME}/toolchains/llvm/prebuilt/windows-x86_64/sysroot)
project(TcpServer)

set(CMAKE_CXX_STANDARD 14)

include_directories(arm64-android/include)
link_directories(arm64-android/lib)

add_executable(${PROJECT_NAME} main.cpp ToolKits.cpp Serial.cpp SerialMessageCallback.cpp SerialReceiveHandle.cpp SystemMessageCallback.cpp)
target_link_libraries(${PROJECT_NAME} -lhv_static -llog -static-libstdc++)

add_executable(TcpClient_test TcpClient_test.cpp)
target_link_libraries(TcpClient_test -lhv_static -llog -static-libstdc++)
