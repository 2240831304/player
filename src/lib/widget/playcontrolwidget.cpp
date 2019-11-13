
#include "playcontrolwidget.h"
#include "systempath.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QProgressBar>
#include <QDebug>
#include "progressbar.h"
#include <QLabel>
#include <QTime>

namespace eink{

    class PlayControlWidgetPrivate
    {
        public:
            ~PlayControlWidgetPrivate(){}

            ProgressBar *playProgressBar;
            QPushButton *playStateBut;
            bool isPlaying;
            QLabel *currentPlayTimeLab;
            QLabel *totleTimeLab;

    };


    PlayControlWidget::PlayControlWidget(QWidget *parent)
        : QWidget(parent),d(new PlayControlWidgetPrivate)
    {
        d->isPlaying = false;
        setupUI();
    }

    PlayControlWidget::~PlayControlWidget()
    {
        if(d)
            delete d;
        d = NULL;
    }

    void PlayControlWidget::setupUI()
    {
        d->playProgressBar = new ProgressBar(this);
        connect(d->playProgressBar,SIGNAL(changePlayTimePos(int )),
                        SIGNAL(changePlayTimePos(int )));

        d->playStateBut = new QPushButton(this);
        connect(d->playStateBut,SIGNAL(clicked(bool)),this,SLOT(playStateClickedSlot(bool)));

        QString imagePathStr = SystemPath::getResourcePath() + "/pic/nextplay.png";
        QPushButton *nextPlayBut = new QPushButton(this);
        nextPlayBut->setIcon(QIcon(imagePathStr));
        connect(nextPlayBut,SIGNAL(clicked()),SIGNAL(nextPlaySig()));

        imagePathStr = SystemPath::getResourcePath() + "/pic/lastplay.png";
        QPushButton *lastPlayBut = new QPushButton(this);
        lastPlayBut->setIcon(QIcon(imagePathStr));
        connect(lastPlayBut,SIGNAL(clicked()),SIGNAL(lastPlaySig()));

        //time
        d->totleTimeLab = new QLabel(this);
        d->currentPlayTimeLab = new QLabel(this);

        QHBoxLayout *hLayout = new QHBoxLayout;
        hLayout->addWidget(d->currentPlayTimeLab,0,Qt::AlignLeft);
        hLayout->addWidget(lastPlayBut,0,Qt::AlignHCenter);
        hLayout->addWidget(d->playStateBut,0,Qt::AlignHCenter);
        hLayout->addWidget(nextPlayBut,0,Qt::AlignHCenter);
        hLayout->addWidget(d->totleTimeLab,0,Qt::AlignRight);

        QVBoxLayout *mainLayout = new QVBoxLayout(this);
        mainLayout->addWidget(d->playProgressBar);
        mainLayout->addLayout(hLayout);
        setLayout(mainLayout);

    }

    void PlayControlWidget::setPlayState(bool flag)
    {
        QString tempStr;
        if(flag)
            tempStr = SystemPath::getResourcePath() + "/pic/playing.png";
        else
            tempStr = SystemPath::getResourcePath() + "/pic/startplay.png";
         d->playStateBut->setIcon(QIcon(tempStr));

        d->isPlaying = flag;
    }


    void PlayControlWidget::playStateClickedSlot(bool)
    {
        if(d->isPlaying)
            d->isPlaying = false;
        else
            d->isPlaying = true;

        QString tempStr;
        if(d->isPlaying)
            tempStr = SystemPath::getResourcePath() + "/pic/playing.png";
        else
            tempStr = SystemPath::getResourcePath() + "/pic/startplay.png";
         d->playStateBut->setIcon(QIcon(tempStr));

         emit playStateClickedSig(d->isPlaying);

    }

    void PlayControlWidget::setFileTotleTime(int lenth)
    {
        int timeMesLenth = lenth / 1000;
        d->playProgressBar->setMaximum(timeMesLenth);
        d->playProgressBar->setMinimum(0);

        int timeLenth = lenth / 1000000;
        int hour = timeLenth / 3600;

        QTime time(0,0,0,0);
        QString str;
        time = time.addMSecs(lenth / 1000);
        if(hour > 0)
            str = time.toString("hh:mm:ss");
        else
            str = time.toString("mm:ss");

        d->totleTimeLab->setText(str);
    }

    void PlayControlWidget::setCurrentPlayTime(int lenth)
    {
        int timeMesLenth = lenth / 1000;
        d->playProgressBar->setMaximum(timeMesLenth);
        int timeLenth = lenth / 1000000;
        int hour = timeLenth / 3600;

        QTime time(0,0,0,0);
        QString str;
        time = time.addMSecs(lenth / 1000);
        if(hour > 0)
            str = time.toString("hh:mm:ss");
        else
            str = time.toString("mm:ss");

        d->currentPlayTimeLab->setText(str);
    }

    void PlayControlWidget::setCurrentPlayTimeMillisecond(int lenth)
    {
        d->playProgressBar->setValue(lenth);

        int timeLenth = lenth / 1000;
        int hour = timeLenth / 3600;

        QTime time(0,0,0,0);
        QString str;
        time = time.addMSecs(lenth);
        if(hour > 0)
            str = time.toString("hh:mm:ss");
        else
            str = time.toString("mm:ss");

        d->currentPlayTimeLab->setText(str);
    }



}