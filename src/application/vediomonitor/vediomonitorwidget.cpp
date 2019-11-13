
#include "vediomonitorwidget.h"
#include "vediomonitorplaywidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QDebug>
#include "vediomonitorthread.h"
#include "cameragrab.h"
#include "sdlplaywidget.h"


class VedioMonitorWidgetPrivate
{
    public:
       VedioMonitorWidgetPrivate()
       {
            playWid = NULL;
            cameraGrab = NULL;
            vedioMonitorThread = NULL;
            sdlPlayUI = NULL;
       }

       QPushButton *startBut;
       QPushButton *saveBut;
       QPushButton *stopBut;
       VedioMonitorPlayWidget *playWid;
       eink::CameraGrab *cameraGrab;
       eink::VedioMonitorThread *vedioMonitorThread;
       eink::SdlPlayWidget *sdlPlayUI;
};

VedioMonitorWidget::VedioMonitorWidget(QWidget *parent)
    :QWidget(parent),d(new VedioMonitorWidgetPrivate)
{
    setupUI();
}

VedioMonitorWidget::~VedioMonitorWidget()
{
    if(!d->cameraGrab)
        delete d->cameraGrab;
    d->cameraGrab = NULL;

    if(d)
        delete d;
    d = NULL;
}

void VedioMonitorWidget::ExitApplication()
{
    if(d->vedioMonitorThread && d->vedioMonitorThread->isRunning()){
        if(d->cameraGrab)
            d->cameraGrab->setWorkingState(false);
        d->vedioMonitorThread->quit();
        d->vedioMonitorThread->wait();
    }
}

void VedioMonitorWidget::setupUI()
{
    d->playWid = new VedioMonitorPlayWidget(this);

    d->startBut = new QPushButton(this);
    d->startBut->setText(QString::fromLocal8Bit("开始抓频"));
    connect(d->startBut,SIGNAL(clicked()),this,SLOT(startSlot()));

    d->stopBut = new QPushButton(this);
    d->stopBut->setText(QString::fromLocal8Bit("停止抓频"));
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

void VedioMonitorWidget::startSlot()
{
    qDebug() << "VedioMonitorWidget::startSlot()!!!!!!!!!!!!!!!!!!!!!!";
    d->startBut->setEnabled(false);
    d->saveBut->setEnabled(true);

    //设置sdl播放窗口
    if(!d->sdlPlayUI) {
        void *pWnd = (void *)d->playWid->winId();
        d->sdlPlayUI = new eink::SdlPlayWidget;
        d->sdlPlayUI->setSdlUISize(d->playWid->width(),d->playWid->height());
        d->sdlPlayUI->createPlayWindow(pWnd);
        //d->sdlPlayUI->test();
    }

    if(!d->cameraGrab) {
        d->cameraGrab = new eink::CameraGrab;
        connect(d->cameraGrab,SIGNAL(sendImageSig(QImage)),this,SLOT(receiveImageSlot(QImage)));
    }
    d->cameraGrab->setSdlPlayWidget(d->sdlPlayUI);

    if(!d->vedioMonitorThread){
        d->vedioMonitorThread = new eink::VedioMonitorThread(this);
        d->vedioMonitorThread->setGrabType(d->cameraGrab);
    }
    d->vedioMonitorThread->start();
}

void VedioMonitorWidget::stopSlot()
{
    if(d->cameraGrab)
        d->cameraGrab->setWorkingState(false);
    if(d->vedioMonitorThread && d->vedioMonitorThread->isRunning()){
        d->vedioMonitorThread->quit();
        d->vedioMonitorThread->wait();
    }
    d->startBut->setEnabled(true);
    d->saveBut->setEnabled(false);
}

void VedioMonitorWidget::saveSlot()
{
    d->saveBut->setEnabled(false);
    if(d->cameraGrab)
        d->cameraGrab->saveFile();
}

void VedioMonitorWidget::receiveImageSlot(QImage image)
{
    d->playWid->setImage(image);

}