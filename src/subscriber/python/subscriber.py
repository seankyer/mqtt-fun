import paho.mqtt.client as mqtt

broker = "localhost"
topic = "sensors/temperature"
lwt = "devices/device1/status"

def on_message(client, userdata, msg):
    print(f"Received {msg.payload.decode()} on topic `{msg.topic}`")

client = mqtt.Client(client_id="Logger", callback_api_version=mqtt.CallbackAPIVersion.VERSION2)
client.connect(broker)
client.subscribe(topic, qos=1)
client.subscribe(lwt, qos=1)
client.on_message = on_message

client.loop_forever()
