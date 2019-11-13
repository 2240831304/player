

#include "signalplugin.h"

void SignalPlugin::sendMessage(const QString &type,const QString &flag)
{
    emit signalMessage(type,flag);
}


void SignalPlugin::sendData(const QString &type,const QString &flag,const QString &data)
{
    emit signalData(type,flag,data);
}

QString SignalPlugin::getTestFunction()
{
    return "test plugin";
}
