adb root
adb shell killall -9 TcpServer
adb shell touch /data/local/demo.lock
adb shell /data/local/addition/TcpServer
