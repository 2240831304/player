
#include "mqttpubmessage.h"
#include <iostream>
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "MQTTClient.h"


#define ADDRESS     "tcp://localhost:1883"
#define CLIENTID    "ExampleClientPub"
#define TOPIC       "topic"
#define PAYLOAD     "Hello World!nigechacha"
#define QOS         1
#define TIMEOUT     10000L


class MqttPubMessagePrivate{
    public:

};


MqttPubMessage::MqttPubMessage()
    : d(new MqttPubMessagePrivate)
{
}

MqttPubMessage::~MqttPubMessage()
{
    if(d) {
        delete d;
        d = NULL;
    }
}

void MqttPubMessage::testPubMes()
{

#if 1
    MQTTClient client;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    MQTTClient_message pubmsg = MQTTClient_message_initializer;
    MQTTClient_deliveryToken token;
    int rc;

    MQTTClient_create(&client, ADDRESS, CLIENTID,
        MQTTCLIENT_PERSISTENCE_NONE, NULL);
    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;

    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS)
    {
        printf("Failed to connect, return code %d\n", rc);
        exit(EXIT_FAILURE);
    }

    std::string str = "Hello World!";
    pubmsg.payload = (void*) PAYLOAD;
    pubmsg.payloadlen = strlen(PAYLOAD);
    pubmsg.qos = QOS;
    pubmsg.retained = 0;

    MQTTClient_publishMessage(client, TOPIC, &pubmsg, &token);
    printf("Waiting for up to %d seconds for publication of %s\n"
            "on topic %s for client with ClientID: %s\n",
            (int)(TIMEOUT/1000), PAYLOAD, TOPIC, CLIENTID);

    rc = MQTTClient_waitForCompletion(client, token, TIMEOUT);
    printf("Message with delivery token %d delivered\n", token);

    MQTTClient_disconnect(client, 10000);
    MQTTClient_destroy(&client);

#endif
}