#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>     // for sleep()
#include "MQTTClient.h"

#define ADDRESS     "tcp://localhost:1883"
#define CLIENTID    "Logger"
#define TOPIC1      "sensors/temperature"
#define TOPIC2      "devices/device1/status"
#define QOS         1

// Callback called when a message arrives
int message_arrived(void *context, char *topicName, int topicLen, MQTTClient_message *message) {
    printf("Received %.*s on topic `%s`\n", message->payloadlen, (char*)message->payload, topicName);

    MQTTClient_freeMessage(&message);
    MQTTClient_free(topicName);
    return 1; // message handled
}

// Called when connection is lost
void connection_lost(void *context, char *cause) {
    printf("Connection lost: %s\n", cause);
}

int main(int argc, char* argv[]) {
    MQTTClient client;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    int rc;

    MQTTClient_create(&client, ADDRESS, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL);

    // Set callbacks
    MQTTClient_setCallbacks(client, NULL, connection_lost, message_arrived, NULL);

    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;

    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS) {
        printf("Failed to connect, return code %d\n", rc);
        exit(EXIT_FAILURE);
    }

    // Subscribe to both topics with QoS 1
    MQTTClient_subscribe(client, TOPIC1, QOS);
    MQTTClient_subscribe(client, TOPIC2, QOS);

    // Loop forever to keep the client alive and receiving messages
    while (1) {
        sleep(1);
    }

    MQTTClient_disconnect(client, 10000);
    MQTTClient_destroy(&client);
    return rc;
}
