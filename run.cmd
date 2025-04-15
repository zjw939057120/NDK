cd cmake-build-debug
cmake --build . --target all -- -j 64

adb root
adb shell killall -9 tinyplay
adb push tinyplay /data/local
adb shell chmod -R 755  /data/local/tinyplay
::adb shell /data/local/tinyplay
