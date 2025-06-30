use rumqttc::{MqttOptions, AsyncClient, QoS, LastWill, Event};
use tokio::time::{sleep, Duration};
use rand::Rng;

#[tokio::main]
async fn main() {
    let mut mqttoptions = MqttOptions::new("TempSensor", "localhost", 1883);
    mqttoptions.set_keep_alive(Duration::from_secs(20));

    // Set Last Will and Testament (LWT)
    mqttoptions.set_last_will(LastWill::new("devices/device1/status", "offline", QoS::AtLeastOnce, true));

    let (client, mut eventloop) = AsyncClient::new(mqttoptions, 10);

    // Spawn task to drive MQTT event loop
    tokio::spawn(async move {
        loop {
            match eventloop.poll().await {
                Ok(Event::Incoming(_)) => {}
                Ok(Event::Outgoing(_)) => {}
                Err(e) => {
                    eprintln!("Event loop error: {}", e);
                    break;
                }
            }
        }
    });

    // Clear retained message and publish online status
    client.publish("sensors/temperature", QoS::AtLeastOnce, true, "").await.unwrap();
    client.publish("devices/device1/status", QoS::AtLeastOnce, true, "online").await.unwrap();

    loop {
        let temp = 20.0 + rand::rng().random_range(-5.0..5.0);
        let payload = format!("{:.2}°C", temp);

        client.publish("sensors/temperature", QoS::AtLeastOnce, false, payload.as_bytes()).await.unwrap();
        println!("Published temperature: {}", payload);

        sleep(Duration::from_secs(2)).await;
    }
}
