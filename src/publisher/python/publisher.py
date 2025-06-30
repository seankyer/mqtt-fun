import paho.mqtt.client as mqtt
import time
import random

broker = "localhost"
topic = "sensors/temperature"
lwt = "devices/device1/status"

client = mqtt.Client(client_id="TempSensor", callback_api_version=mqtt.CallbackAPIVersion.VERSION2)
client.will_set(
    topic=lwt,
    payload="offline",
    qos=1,
    retain=True
)
client.connect(broker)
client.loop_start()

# Clear retained message and declare online
client.publish(topic, "", qos=1, retain=False)
client.publish(lwt, "online", qos=1, retain=True)

# Enter sensor loop
while True:
    temp = round(20 + random.uniform(-5, 5), 2)
    client.publish(topic, f"{temp}°C", qos=1, retain=False)
    print(f"Published temperature: {temp}°C")
    time.sleep(2)
