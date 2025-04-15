cd cmake-build-debug
cmake --build . --target all -- -j 64

adb root
adb shell killall -9 pcm_converter
adb push pcm_converter /data/local
adb shell chmod -R 755  /data/local/*
adb shell /data/local/pcm_converter
adb shell /data/local/tinyplay /usr/samples/output_48k_stereo.pcm  --raw 2 48000 16 1024000 -D 0 -d 0
::adb shell /data/local/tinyplay /usr/samples/origin.g711a  --raw 2 48000 16 1024000 -D 0 -d 0
::adb shell /data/local/tinyplay  /usr/samples/origin.raw  --raw 2 48000 16 1024000 -D 0 -d 0
::adb shell /data/local/tinyplay  /usr/samples/Stereo48000.wav  --raw 2 48000 16 1024000 -D 0 -d 0

