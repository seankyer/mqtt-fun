use rumqttc::{MqttOptions, AsyncClient, QoS, Event, Packet};
use tokio::time::Duration;

#[tokio::main]
async fn main() {
    let mut mqttoptions = MqttOptions::new("Logger", "localhost", 1883);
    mqttoptions.set_keep_alive(Duration::from_secs(20));

    let (client, mut eventloop) = AsyncClient::new(mqttoptions, 10);

    client.subscribe("sensors/temperature", QoS::AtLeastOnce).await.unwrap();
    client.subscribe("devices/device1/status", QoS::AtLeastOnce).await.unwrap();

    loop {
        let event = eventloop.poll().await;
        match event {
            Ok(Event::Incoming(Packet::Publish(publish))) => {
                let payload = String::from_utf8_lossy(&publish.payload);
                println!("Received {} on topic `{}`", payload, publish.topic);
            }
            Ok(_) => {}
            Err(e) => {
                eprintln!("Error in event loop: {}", e);
                break;
            }
        }
    }
}
