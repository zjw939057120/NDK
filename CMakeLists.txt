cmake_minimum_required(VERSION 3.18)

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

project(tinyalsa)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_STANDARD 99)

include_directories(include)

set(libtinyalsa
        mixer.c
        mixer_hw.c
        mixer_plugin.c
        pcm.c
        pcm_hw.c
        pcm_plugin.c
        snd_utils.c
        )

add_executable(tinyplay tinyplay.c ${libtinyalsa})
target_link_libraries(tinyplay -ldl)

add_executable(tinycap tinycap.c ${libtinyalsa})
target_link_libraries(tinycap -ldl)

add_executable(tinymix tinymix.c ${libtinyalsa})
target_link_libraries(tinymix -ldl)

add_executable(tinyhostless tinyhostless.c ${libtinyalsa})
target_link_libraries(tinyhostless -ldl)

add_executable(tinypcminfo tinypcminfo.c ${libtinyalsa})
target_link_libraries(tinypcminfo -ldl)