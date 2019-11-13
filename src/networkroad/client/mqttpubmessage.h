
#ifndef MQTTPUBMESSAGE_H
#define MQTTPUBMESSAGE_H


class MqttPubMessagePrivate;
class MqttPubMessage {
    public:
        MqttPubMessage();
        ~MqttPubMessage();

        void testPubMes();


    private:
        MqttPubMessagePrivate *d;


};

#endif