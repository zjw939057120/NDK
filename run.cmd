cd cmake-build-release
cmake --build . --target all -- -j 64

adb root
adb shell killall -9 TcpServer
adb push TcpServer /data/local/addition/
adb shell chmod -R 755  /data/local/addition
adb shell /data/local/addition/TcpServer
