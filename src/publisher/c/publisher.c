#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "MQTTClient.h"

#define ADDRESS     "tcp://localhost:1883"
#define CLIENTID    "TempSensor"
#define TOPIC       "sensors/temperature"
#define LWT_TOPIC   "devices/device1/status"
#define QOS         1
#define TIMEOUT     10000L

// Generate random float in range [min, max]
float rand_float(float min, float max) {
    float scale = rand() / (float) RAND_MAX; // 0 to 1
    return min + scale * (max - min);
}

int main(int argc, char* argv[]) {
    MQTTClient client;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    MQTTClient_willOptions will_opts = MQTTClient_willOptions_initializer;
    int rc;

    srand(time(NULL)); // seed RNG

    MQTTClient_create(&client, ADDRESS, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL);

    // Setup Last Will and Testament (LWT)
    will_opts.topicName = LWT_TOPIC;
    will_opts.message = "offline";
    will_opts.qos = QOS;
    will_opts.retained = 1;

    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;
    conn_opts.will = &will_opts;

    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS) {
        printf("Failed to connect, return code %d\n", rc);
        exit(EXIT_FAILURE);
    }

    // Clear retained message on temperature topic (publish empty retained)
    MQTTClient_publish(client, TOPIC, 0, "", QOS, 0, NULL);

    // Publish "online" status retained message on LWT topic
    MQTTClient_publish(client, LWT_TOPIC, strlen("online"), "online", QOS, 1, NULL);

    while(1) {
        float temp = rand_float(15.0f, 25.0f); // 20 ± 5 °C approx
        char payload[20];
        snprintf(payload, sizeof(payload), "%.2f°C", temp);

        MQTTClient_publish(client, TOPIC, strlen(payload), payload, QOS, 0, NULL);
        printf("Published temperature: %s\n", payload);

        sleep(2);
    }

    MQTTClient_disconnect(client, TIMEOUT);
    MQTTClient_destroy(&client);
    return rc;
}
