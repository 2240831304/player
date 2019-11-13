
#include "capturewidget.h"
#include "captureplaywidget.h"
#include "desktopgrab.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QDebug>
#include "capturethread.h"
#include "sdlplaywidget.h"


class CaptureWidgetPrivate
{
    public:
       CaptureWidgetPrivate()
       {
            playWid = NULL;
            desktopGrab = NULL;
            captureThread = NULL;
            sdlPlayUI = NULL;
       }

       QPushButton *startBut;
       QPushButton *saveBut;
       QPushButton *rtmpBut;
       QPushButton *stopBut;
       CapturePlayWidget *playWid;
       eink::DesktopGrab *desktopGrab;
       eink::CaptureThread *captureThread;
       eink::SdlPlayWidget *sdlPlayUI;
};

CaptureWidget::CaptureWidget(QWidget *parent)
    :QWidget(parent),d(new CaptureWidgetPrivate)
{
    setupUI();
}

CaptureWidget::~CaptureWidget()
{
    if(!d->desktopGrab)
        delete d->desktopGrab;
    d->desktopGrab = NULL;

    if(d)
        delete d;
    d = NULL;
}

void CaptureWidget::ExitApplication()
{
    if(d->captureThread && d->captureThread->isRunning()){
        if(d->desktopGrab)
            d->desktopGrab->setWorkingState(false);
        d->captureThread->quit();
        d->captureThread->wait();
    }
}

void CaptureWidget::setupUI()
{
    d->playWid = new CapturePlayWidget(this);

    d->startBut = new QPushButton(this);
    d->startBut->setText(QString::fromLocal8Bit("开始抓频"));
    connect(d->startBut,SIGNAL(clicked()),this,SLOT(startSlot()));

    d->stopBut = new QPushButton(this);
    d->stopBut->setEnabled(false);
    d->stopBut->setText(QString::fromLocal8Bit("停止抓频"));
    connect(d->stopBut,SIGNAL(clicked()),this,SLOT(stopSlot()));

    d->saveBut = new QPushButton(this);
    d->saveBut->setEnabled(false);
    d->saveBut->setText(QString::fromLocal8Bit("保存视频"));
    connect(d->saveBut,SIGNAL(clicked()),this,SLOT(saveSlot()));

    d->rtmpBut = new QPushButton(this);
    d->rtmpBut->setEnabled(false);
    d->rtmpBut->setText(QString::fromLocal8Bit("推送视频"));
    connect(d->rtmpBut,SIGNAL(clicked()),this,SLOT(rtmpSlot()));

    QHBoxLayout *hLayout = new QHBoxLayout;
    hLayout->addWidget(d->startBut);
    hLayout->addWidget(d->saveBut);
    hLayout->addWidget(d->rtmpBut);
    hLayout->addWidget(d->stopBut);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(d->playWid,10);
    mainLayout->addLayout(hLayout,1);
    setLayout(mainLayout);
}

void CaptureWidget::startSlot()
{
    qDebug() << "CaptureWidget::startSlot()!!!!!!!!!!!!!!!!!!!!!!";
    d->startBut->setEnabled(false);

    if(!d->sdlPlayUI){
        //设置sdl播放窗口
        void *pWnd = (void *)d->playWid->winId();
        d->sdlPlayUI = new eink::SdlPlayWidget;
        d->sdlPlayUI->setSdlUISize(d->playWid->width(),d->playWid->height());
        d->sdlPlayUI->createPlayWindow(pWnd);
    }

    if(!d->desktopGrab) {
        d->desktopGrab = new eink::DesktopGrab;
        connect(d->desktopGrab,SIGNAL(sendImageSig(QImage)),this,SLOT(receiveImageSlot(QImage)));
    }
    d->desktopGrab->setSdlPlayWidget(d->sdlPlayUI);

    if(!d->captureThread){
        d->captureThread = new eink::CaptureThread(this);
        d->captureThread->setGrabType(d->desktopGrab);
    }
    d->captureThread->start();

    d->saveBut->setEnabled(true);
    d->rtmpBut->setEnabled(true);
    d->stopBut->setEnabled(true);
}

void CaptureWidget::stopSlot()
{
    if(d->desktopGrab)
        d->desktopGrab->setWorkingState(false);
    if(d->captureThread && d->captureThread->isRunning()){
        d->captureThread->quit();
        d->captureThread->wait();
    }

    d->saveBut->setEnabled(false);
    d->rtmpBut->setEnabled(false);
    d->stopBut->setEnabled(false);
    d->startBut->setEnabled(true);
}

void CaptureWidget::saveSlot()
{
    d->saveBut->setEnabled(false);
    if(d->desktopGrab)
        d->desktopGrab->saveFile();
}

void CaptureWidget::rtmpSlot()
{
    d->rtmpBut->setEnabled(false);
    if(d->desktopGrab)
        d->desktopGrab->rtmpFlugVedio();
}


void CaptureWidget::receiveImageSlot(QImage image)
{
    d->playWid->setImage(image);
}