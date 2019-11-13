
#ifndef PLUGININTERFACE_H
#define PLUGININTERFACE_H

#include <QString>

QT_BEGIN_NAMESPACE

#define PluginInterface_iid "org.qt-project.Qt.PluginInterface"

class PluginInterface
{
    public:
        virtual ~PluginInterface() {}

        virtual void sendMessage(const QString &,const QString &) =0;
        virtual void sendData(const QString &,const QString &,const QString &)=0;
        virtual QString getTestFunction()=0;

};


Q_DECLARE_INTERFACE(PluginInterface,PluginInterface_iid)
QT_END_NAMESPACE

#endif