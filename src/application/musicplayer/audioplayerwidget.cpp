
#include "audioplayerwidget.h"
#include <QPainter>
#include "systempath.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMouseEvent>
#include <QRect>
#include "filemanagerview.h"
#include <QDebug>
#include <QModelIndex>
#include <QApplication>
#include <QTime>
#include "playerwidget.h"

AudioPlayerWidget::AudioPlayerWidget(QWidget *parent)
    : QWidget(parent),isStartPlay(false)
{
    setWindowFlags(Qt::FramelessWindowHint);

    nextPlayFirstClicked = true;
    lastPlayFirstClicked = true;

    setupUI();

}

AudioPlayerWidget::~AudioPlayerWidget()
{
}

void AudioPlayerWidget::setupUI()
{
    playWidget = new PlayerWidget(this);
    connect(playWidget,SIGNAL(playNextMusicSig()),this,SLOT(playNextMusicSlot()));
    connect(playWidget,SIGNAL(playLastMusicSig()),this,SLOT(playLastMusicSlot()));

    fileView = new FileManagerView(this);
    fileView->setFileType(FILE_TYPE::FILE_AUDIO);
    connect(fileView,SIGNAL(doubleClicked(const QModelIndex &)),
        this,SLOT(doubleClickedsLOT(const QModelIndex &)));

    QHBoxLayout *hLayout = new QHBoxLayout;
    hLayout->setContentsMargins(0,1,0,0);
    hLayout->addWidget(playWidget,3);
    hLayout->setSpacing(0);
    hLayout->addWidget(fileView,1);

    QVBoxLayout *mainVlayout = new QVBoxLayout(this);
    mainVlayout->setContentsMargins(1,1,1,1);
    mainVlayout->setSpacing(0);
    mainVlayout->addLayout(hLayout);
    setLayout(mainVlayout);
}

void AudioPlayerWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.drawLine(0, 0, this->width() - 1, 0);
    painter.drawLine(0, 0, 0, this->height() - 1);
    painter.drawLine(this->width() - 1, 0, this->width() - 1, this->height() - 1);
    painter.drawLine(0, this->height() - 1, this->width() - 1, this->height() - 1);
}

void AudioPlayerWidget::quitApp()
{
    qApp->quit();
}

void AudioPlayerWidget::resetEvn()
{
    playWidget->stop();
}


void AudioPlayerWidget::doubleClickedsLOT(const QModelIndex &index)
{
    static QTime startTime = QTime::currentTime();

    QString filePath = index.data(1).toString();
    qDebug() << "AudioPlayerWidget::doubleClickedsLOT selected file path===" << filePath;

    if(isStartPlay) {
        QTime curTime = QTime::currentTime();
        if(startTime.msecsTo(curTime) < 1000) {
            startTime = curTime;
            return;
        }
    }

    resetEvn();

    playWidget->setFilePath(filePath);
    playWidget->play();

    isStartPlay = true;
}

void AudioPlayerWidget::ExitApplication()
{
    resetEvn();
}

void AudioPlayerWidget::playNextMusicSlot()
{
    static QTime startTime = QTime::currentTime();

    if(nextPlayFirstClicked) {
        nextPlayFirstClicked = false;
    } else {
        QTime curTime = QTime::currentTime();
        if(startTime.msecsTo(curTime) < 1500) {
            startTime = curTime;
            return;
        }
    }

    if(isStartPlay)
        fileView->sendNextFileMes();
}

void AudioPlayerWidget::playLastMusicSlot()
{
    static QTime startTime = QTime::currentTime();

    if(lastPlayFirstClicked) {
        lastPlayFirstClicked = false;
    } else {
        QTime curTime = QTime::currentTime();
        if(startTime.msecsTo(curTime) < 1500) {
            startTime = curTime;
            return;
        }
    }

    if(isStartPlay)
        fileView->sendLastFileMes();
}

void AudioPlayerWidget::systemMessage(const QString &msg, const QByteArray &data)
{
    if(msg == "AudioPlayFinished()") {
    }
}

