
#ifndef SIGNALPLUGIN_H
#define SIGNALPLUGIN_H

#include <QObject>
#include <QtPlugin>

#include "plugininterface.h"

class SignalPlugin : public QObject, public PluginInterface
{
    Q_OBJECT
    //Q_PLUGIN_METADATA(IID "org.qt-project.Qt.PluginInterface" FILE "SignalPlugin.json")
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.PluginInterface")
    Q_INTERFACES(PluginInterface)
    public:
        virtual void sendMessage(const QString &,const QString &);
        virtual void sendData(const QString &,const QString &,const QString &);

        virtual QString getTestFunction();

    signals:
        void signalMessage(const QString &,const QString &);
        void signalData(const QString &,const QString &,const QString &);

    private:

};

#endif