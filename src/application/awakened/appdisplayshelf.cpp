

#include "appdisplayshelf.h"
#include "systempath.h"
#include <QPainter>
#include <QListWidget>
#include <QDebug>
#include <QStackedWidget>
#include <QImage>
#include <QVBoxLayout>
#include "apptitlewidget.h"
#include <QListWidgetItem>
#include "playermainwidget.h"
#include <QTabWidget>
#include <QMap>
#include "baseapp.h"
#include "tabbar.h"
#include "audioplayerwidget.h"
#include "vediomonitorwidget.h"
#include <QStringList>
#include <QList>
#include "capturewidget.h"
#include "imageprocessorwidget.h"
#include "rtmplivewidget.h"

namespace eink {

    class AppDisplayShelfPrivate {
        public:
            QListWidget *list;
            QStackedWidget *stack;
            QStringList appList;
            QTabWidget *tabWidget;
            QStringList openAppNameList;
            QList<BaseApp *> workingAppList;

    };

    AppDisplayShelf::AppDisplayShelf(QWidget *parent)
        : QWidget(parent),d(new AppDisplayShelfPrivate)
    {
        setupUI();
    }

    AppDisplayShelf::~AppDisplayShelf()
    {
        d->openAppNameList.clear();
        if(d)
            delete d;
        d = NULL;
    }

    void AppDisplayShelf::setupUI()
    {
        d->tabWidget = new QTabWidget(this);
        d->tabWidget->setTabsClosable(true);
        connect(d->tabWidget,SIGNAL(tabCloseRequested(int)),this,SLOT(removeSubTabSlot(int)));
        //connect(d->tabWidget,SIGNAL(currentChanged(int)),SLOT(setCurrentIndex(int)));

        //TabBar *bar = new TabBar(this);
        //d->tabWidget->setWidgetTabBar(bar);

        addApplication("homepage","首页");

        QVBoxLayout *mainLayout = new QVBoxLayout(this);
        mainLayout->setContentsMargins(0,0,0,0);
        mainLayout->addWidget(d->tabWidget);
        setLayout(mainLayout);
    }

    void AppDisplayShelf::addApplication(const QString &appUSname,const QString &appZHname)
    {
        if(d->openAppNameList.indexOf(appUSname) > 0)
            return;

        d->openAppNameList.push_back(appUSname);

        if("homepage" == appUSname) {
            //初始化主页窗口
            //homeTitleWidget->setCloseButShow(false);

            HomePageWidget *homeWidget = new HomePageWidget(this);
            d->tabWidget->addTab(homeWidget,appZHname);
            d->tabWidget->tabBar()->setTabButton(d->tabWidget->indexOf(homeWidget),QTabBar::RightSide,NULL);

        } else if("qtplayer" == appUSname) {
            //显示qt播放器
            PlayerMainWidget *qtPlayerWidget = new PlayerMainWidget;
            int index = d->tabWidget->addTab(qtPlayerWidget,appZHname);
            d->workingAppList.push_back(qtPlayerWidget);
            d->tabWidget->setCurrentIndex(index);
        } else if("musicplayer" == appUSname) {
            //显示音乐播放器
            AudioPlayerWidget *audioPlayerWidget = new AudioPlayerWidget;
            int index = d->tabWidget->addTab(audioPlayerWidget,appZHname);
            d->workingAppList.push_back(audioPlayerWidget);
            d->tabWidget->setCurrentIndex(index);
        } else if("vediomonitor" == appUSname) {
            //视频监控
            VedioMonitorWidget *vedioMonitorWidget = new VedioMonitorWidget;
            int index = d->tabWidget->addTab(vedioMonitorWidget,appZHname);
            d->workingAppList.push_back(vedioMonitorWidget);
            d->tabWidget->setCurrentIndex(index);
        } else if("desktopcapture" == appUSname) {
            CaptureWidget *captureWidget = new CaptureWidget;
            int index = d->tabWidget->addTab(captureWidget,appZHname);
            d->workingAppList.push_back(captureWidget);
            d->tabWidget->setCurrentIndex(index);
        } else if("pictureprocessor" == appUSname) {
            ImageProcessorWidget *imageProcessorWidget = new ImageProcessorWidget;
            int index = d->tabWidget->addTab(imageProcessorWidget,appZHname);
            d->workingAppList.push_back(imageProcessorWidget);
            d->tabWidget->setCurrentIndex(index);
        } else if("rtmplive" == appUSname){
            RtmpliveWidget *rtmpLiveWidget = new RtmpliveWidget;
            int index = d->tabWidget->addTab(rtmpLiveWidget,appZHname);
            d->workingAppList.push_back(rtmpLiveWidget);
            d->tabWidget->setCurrentIndex(index);
        }

    }

    void AppDisplayShelf::removeSubTabSlot(int index)
    {
        qDebug() << "AppDisplayShelf::removeSubTabSlot the index ====" << index;
        d->tabWidget->removeTab(index);
        d->openAppNameList.removeAt(index);

        int listIndex = index -1;
        BaseApp *appWidget = d->workingAppList[listIndex];
        if(appWidget) {
            appWidget->ExitApplication();
            delete appWidget;
        }
        d->workingAppList.removeAt(listIndex);
    }

    void AppDisplayShelf::closeApps()
    {
        for(int i = 0;i < d->workingAppList.size();i++)
        {
            BaseApp *appWidget = d->workingAppList[i];
            if(appWidget) {
                appWidget->ExitApplication();
                delete appWidget;
                appWidget = NULL;
            }
        }
        d->workingAppList.clear();
    }



    //主页窗口
    HomePageWidget::HomePageWidget(QWidget *parent)
        : QWidget(parent)
    {
    }

    HomePageWidget::~HomePageWidget()
    {
    }

    void HomePageWidget::paintEvent(QPaintEvent *)
    {
        QPainter painter(this);
		painter.setBrush(Qt::white);
		painter.drawRect(0, 0, this->width(), this->height()); //先画成白色

		QString homePagePic = SystemPath::getResourcePath() + "/pic/homepage.jpg";
        QPixmap pix;
        pix.load(homePagePic);//图片的位置
        //根据窗口的宽高来将图片画在窗口上
        painter.drawPixmap(0,0,this->width(),this->height(),pix);
		//painter.drawText(rect, QString::number(time), QTextOption(Qt::AlignCenter));
    }

}
