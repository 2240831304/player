
#include "rtmplivewidget.h"
#include "rtmpliveplaywidget.h"
#include "lovelive.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QDebug>
#include "rtmplivethread.h"
#include "sdlplaywidget.h"


class RtmpliveWidgetPrivate
{
    public:
       RtmpliveWidgetPrivate()
       {
            playWid = NULL;
            loveLive = NULL;
            loveLiveThread = NULL;
            sdlPlayUI = NULL;
       }

       QPushButton *startBut;
       QPushButton *saveBut;
       QPushButton *stopBut;
       RtmplivePlayWidget *playWid;
       eink::LoveLive *loveLive;
       eink::RtmpliveThread *loveLiveThread;
       eink::SdlPlayWidget *sdlPlayUI;
};

RtmpliveWidget::RtmpliveWidget(QWidget *parent)
    :QWidget(parent),d(new RtmpliveWidgetPrivate)
{
    setupUI();
}

RtmpliveWidget::~RtmpliveWidget()
{
    if(!d->loveLive)
        delete d->loveLive;
    d->loveLive = NULL;

    if(d)
        delete d;
    d = NULL;
}

void RtmpliveWidget::ExitApplication()
{
    if(d->loveLiveThread && d->loveLiveThread->isRunning()){
        if(d->loveLive)
            d->loveLive->setWorkingState(false);
        d->loveLiveThread->quit();
        d->loveLiveThread->wait();
    }
}

void RtmpliveWidget::setupUI()
{
    d->playWid = new RtmplivePlayWidget(this);

    d->startBut = new QPushButton(this);
    d->startBut->setText(QString::fromLocal8Bit("观看直播"));
    connect(d->startBut,SIGNAL(clicked()),this,SLOT(startSlot()));

    d->stopBut = new QPushButton(this);
    d->stopBut->setText(QString::fromLocal8Bit("停止观看"));
    connect(d->stopBut,SIGNAL(clicked()),this,SLOT(stopSlot()));

    d->saveBut = new QPushButton(this);
    d->saveBut->setEnabled(false);
    d->saveBut->setText(QString::fromLocal8Bit("保存视频"));
    connect(d->saveBut,SIGNAL(clicked()),this,SLOT(saveSlot()));

    QHBoxLayout *hLayout = new QHBoxLayout;
    hLayout->addWidget(d->startBut);
    hLayout->addWidget(d->saveBut);
    hLayout->addWidget(d->stopBut);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(d->playWid,10);
    mainLayout->addLayout(hLayout,1);
    setLayout(mainLayout);

}

void RtmpliveWidget::startSlot()
{
    qDebug() << "RtmpliveWidget::startSlot()!!!!!!!!!!!!!!!!!!!!!!";
    d->startBut->setEnabled(false);

    //设置sdl播放窗口
    if(!d->sdlPlayUI) {
        void *pWnd = (void *)d->playWid->winId();
        d->sdlPlayUI = new eink::SdlPlayWidget;
        d->sdlPlayUI->setSdlUISize(d->playWid->width(),d->playWid->height());
        d->sdlPlayUI->createPlayWindow(pWnd);
        //d->sdlPlayUI->test();
    }

    if(!d->loveLive) {
        d->loveLive = new eink::LoveLive;
        connect(d->loveLive,SIGNAL(sendImageSig(QImage)),this,SLOT(receiveImageSlot(QImage)));
    }
    d->loveLive->setSdlPlayWidget(d->sdlPlayUI);

    if(!d->loveLiveThread){
        d->loveLiveThread = new eink::RtmpliveThread(this);
        d->loveLiveThread->setGrabType(d->loveLive);
    }

    d->loveLiveThread->start();
    d->saveBut->setEnabled(true);
}

void RtmpliveWidget::stopSlot()
{
    if(d->loveLive)
        d->loveLive->setWorkingState(false);
    if(d->loveLiveThread && d->loveLiveThread->isRunning()){
        d->loveLiveThread->quit();
        d->loveLiveThread->wait();
    }
    d->startBut->setEnabled(true);
    d->saveBut->setEnabled(false);
}

void RtmpliveWidget::saveSlot()
{
    d->saveBut->setEnabled(false);
    if(d->loveLive)
        d->loveLive->saveFile();
}

void RtmpliveWidget::receiveImageSlot(QImage image)
{
    d->playWid->setImage(image);
}
