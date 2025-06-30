# C Implementation of Subscriber

Running the MQTT subscriber in C

## Download C MQTT Library
```
sudo apt update
sudo apt install libpaho-mqtt-dev
```

## Compile C Subscriber
```
gcc subscriber.c -o subscriber.elf -lpaho-mqtt3c
```

## Run C Subscriber
```
./subscriber.elf
```
