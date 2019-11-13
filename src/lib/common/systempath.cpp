
#include <QDebug>
#include <QDir>
#include "systempath.h"

QString SystemPath::getProjectRootPath()
{
    char *path = qgetenv("ROOTDIR").data();
    QString rootDir = QString::fromLatin1(path);

    //qDebug() << "SystemPath::getProjectRootPath()==" << rootDir;
    return rootDir;
}

QString SystemPath::getCurrentPath()
{
    qDebug() << "SystemPath::getCurrentPath()==" << QDir::currentPath();
    return QDir::currentPath();
}

QString SystemPath::getResourcePath()
{
    QString tempStr = getProjectRootPath() + "/resource";
    return tempStr;
}

QString SystemPath::getFilesPath()
{
    QString tempStr = getProjectRootPath() + "/files";
    return tempStr;
}

QString SystemPath::getPluginPath()
{
    QString tempStr = getProjectRootPath() + "/plugins";
    return tempStr;
}

