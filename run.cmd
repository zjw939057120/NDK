cd cmake-build-debug
cmake --build . --target all -- -j 64

adb root
adb shell killall -9 jt905
adb shell rm /data/local/jt905
adb push jt905 /data/local
adb shell chmod -R 755  /data/local/jt905
::adb shell /data/local/jt905
