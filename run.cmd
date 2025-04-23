cd cmake-build-debug
cmake --build . --target all -- -j 64

adb root
adb shell killall -9 stack_limit
adb push stack_limit /data/local
adb shell chmod -R 755  /data/local/*
adb shell ulimit -s 16000
adb shell /data/local/stack_limit