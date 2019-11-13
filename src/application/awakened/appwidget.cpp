
#include "appwidget.h"
#include "systempath.h"
#include <QSettings>
#include <QAction>
#include <QVBoxLayout>
#include <QDebug>

namespace eink{

    AppWidget::AppWidget(QWidget *parent)
        : QWidget(parent)
    {
        setupUI();
    }


    AppWidget::~AppWidget()
    {
    }

    void AppWidget::setupUI()
    {
       listWidget = new QListWidget(this);
       listWidget->setFrameShape(QListWidget::NoFrame);
       listWidget->setViewMode(QListView::IconMode);
       listWidget->setIconSize(QSize(width() - 10,width() - 10));
       listWidget->setSpacing(15);
       connect(listWidget,SIGNAL(itemDoubleClicked(QListWidgetItem *)),
                SIGNAL(itemDoubleClicked(QListWidgetItem *)));

       QVBoxLayout *mainVlayout = new QVBoxLayout(this);
       mainVlayout->setContentsMargins(0,0,0,0);
       mainVlayout->addWidget(listWidget);
       setLayout(mainVlayout);

       appConfigFilePath = SystemPath::getResourcePath() + "/setting/application.conf";
       QSettings conf(appConfigFilePath,QSettings::NativeFormat);
       conf.beginGroup("app");
       QStringList appList = conf.value("apps").toStringList();
       conf.endGroup();

       for(int i = 0;i < appList.size();i++) {
           addAppIcon(appList[i]);
       }

    }

    void AppWidget::addAppIcon(const QString &appStrName)
    {
        QString appImagePath = SystemPath::getResourcePath() + "/pic/apps/" + appStrName + ".png";
        qDebug() << "AppWidget::addAppIcon pic path==" << appImagePath;
        QSettings conf(appConfigFilePath,QSettings::NativeFormat);
        conf.setIniCodec("utf-8");
        conf.beginGroup(appStrName);
        QString appName = conf.value("name").toString();
        conf.endGroup();

        qDebug() << "AppWidget::addAppIcon name=" << appName;

        QListWidgetItem *newItem = new QListWidgetItem(QIcon(appImagePath),appName,listWidget);
        newItem->setData(3,appStrName);
        newItem->setData(4,appName);
        listWidget->addItem(newItem);
    }


}
