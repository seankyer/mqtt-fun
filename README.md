# mqtt-fun
Getting familiar with MQTT. This project includes multiple implementations of a temperature sensor MQTT publisher and subscriber. It is possible to combine any permutation of languages in this program. This project requires an MQTT broker to be running on your localhost.

## About MQTT (Message Queuing Telemetry Transport)

Lightweight, publish-subscribe network protocol for low-bandwidth, high-latency and unreliable networks.

- Publisher: Sends messages (topics + payloads)
- Subscriber: Subscribes to topics to receive matching messages
- Broker/Server: The central server that routes messages from publishers to subscribers

## Getting Started

### Install apt packages
Install Mosquitto Broker
```
sudo apt update
sudo apt install moqsuitto moqsuitto-clients
```
#### Verify it's working
Run broker in background:
```
sudo systemctl start mosquitto
```
Verify the broker is running:
```
moqsuitto_sub -h localhost -t test/topic &
moqsuitto_pub -h localhost -t test/topic -m "hello mqtt"
```
### Install Python packages
```
python3 -m venv .venv
source .venv/bin/activate
pip install -r requirements.txt
```
### Install C Libraries
```
sudo apt update
sudo apt install libpaho-mqtt-dev
```
### Install Rust
```
curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh
source $HOME/.cargo/env
```

