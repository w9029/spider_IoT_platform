arm-linux-g++ *.cpp -DWITH_POSIX -I./Include -I./Include/coap -I./src/ -o final -L./dev/shm/mqtt/arm_mqtt_c/ -L./arm_env -lpaho-mqtt3c -ljsoncpp -lsqlite3 -lpthread -lcoap

