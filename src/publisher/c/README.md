# C Implementation of Publisher

Running the MQTT publisher in C

## Download C MQTT Library
```
sudo apt update
sudo apt install libpaho-mqtt-dev
```

## Compile C Publisher
```
gcc publisher.c -o publisher.elf -lpaho-mqtt3c
```

## Run C Publisher
```
./publisher.elf
```
