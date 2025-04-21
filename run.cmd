cd cmake-build-debug
cmake --build . --target all -- -j 64

adb root
adb shell killall -9 test
adb push test /data/local
adb shell chmod -R 755  /data/local/*
adb shell /data/local/test