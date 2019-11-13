
#include "playerwidget.h"
#include "audioplaymanager.h"
#include "playcontrolwidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDebug>
#include "audiofileinfo.h"
#include "struct.h"
#include <QLabel>
#include <QFileInfo>
#include "lyricwidget.h"
#include "playmonitorthread.h"
#include "sharedvariable.h"
#include <QPluginLoader>

#include "signalplugin.h"
#include "systempath.h"

class PlayerWidgetPrivate
{
    public:
        PlayerWidgetPrivate()
        {
            musicPlayerMgr = NULL;
            audioFileInfoGet = NULL;
            playMonitorThread = NULL;
        }

        ~PlayerWidgetPrivate(){}

        eink::AudioPlayManager *musicPlayerMgr;
        QString filePath;
        eink::PlayControlWidget *playControlWid;
        eink::AudioFileInfo *audioFileInfoGet;
        eink::LyricWidget *LyricWid;

        File_Information_ST fileInfo;

        QLabel *fileNameLab;
        QLabel *fileAuthorLab;
        eink::PlayMonitorThread *playMonitorThread;
        PluginInterface *SigPlugin;
        QPluginLoader pluginLoader;
};

PlayerWidget::PlayerWidget(QWidget *parent)
    : QWidget(parent),d(new PlayerWidgetPrivate)
{
    QString filePath = SystemPath::getPluginPath() + "/libsignalplugin.so";
    d->pluginLoader.setFileName(filePath);
    d->pluginLoader.load();
    QObject *plugin = d->pluginLoader.instance();

    if (plugin) {
        d->SigPlugin = qobject_cast<PluginInterface *>(plugin);
        //d->SigPlugin = qobject_cast<SignalPlugin *> (plugin);
        connect(plugin,SIGNAL(signalMessage(const QString &,const QString &)),
                this,SLOT(signalMessageSlot(const QString &,const QString &)));
        qDebug() << "PlayerWidget::PlayerWidget success test str=" << d->SigPlugin->getTestFunction();
    } else {
        qDebug() << "PlayerWidget::PlayerWidget  faile===================";
    }

    setupUI();
}

PlayerWidget::~PlayerWidget()
{
    if(d->musicPlayerMgr)
        delete d->musicPlayerMgr;
    d->musicPlayerMgr = NULL;

    d->pluginLoader.unload();

    if(d->playMonitorThread)
        delete d->playMonitorThread;
    d->playMonitorThread = NULL;
}

void PlayerWidget::setupUI()
{
    d->fileNameLab = new QLabel(this);
    d->fileAuthorLab = new QLabel(this);

    d->playControlWid = new eink::PlayControlWidget(this);
    d->playControlWid->hide();
    connect(d->playControlWid,SIGNAL(playStateClickedSig(bool)),
                this,SLOT(playStateClickedSlot(bool)));
    connect(d->playControlWid,SIGNAL(nextPlaySig()),SIGNAL(playNextMusicSig()));
    connect(d->playControlWid,SIGNAL(lastPlaySig()),SIGNAL(playLastMusicSig()));
    connect(d->playControlWid,SIGNAL(changePlayTimePos(int )),
                this,SLOT(changePlayTimePosSlot(int )));

    d->LyricWid = new eink::LyricWidget(this);
    //d->LyricWid->hide();

    QVBoxLayout *mainVlayout = new QVBoxLayout(this);
    mainVlayout->setSpacing(0);
    mainVlayout->addWidget(d->fileNameLab,1,Qt::AlignTop | Qt::AlignHCenter);
    mainVlayout->addWidget(d->fileAuthorLab,1,Qt::AlignTop | Qt::AlignHCenter);
    mainVlayout->addWidget(d->LyricWid,12);
    mainVlayout->addWidget(d->playControlWid,1,Qt::AlignBottom);
    setLayout(mainVlayout);

    d->playMonitorThread = new eink::PlayMonitorThread;
}

void PlayerWidget::setFilePath(const QString &path)
{
    d->filePath = path;
}

void PlayerWidget::play()
{
    obtainFileInfotmation();

    if(!d->playMonitorThread)
        d->playMonitorThread = new eink::PlayMonitorThread;
    if(d->playMonitorThread) {
        d->playMonitorThread->setPlayerWidget(this);
        d->playMonitorThread->start();
    }

    if(!d->musicPlayerMgr)
        d->musicPlayerMgr = new eink::AudioPlayManager;

    if(d->musicPlayerMgr) {
        d->musicPlayerMgr->setPlayFilePath(d->filePath);
        d->musicPlayerMgr->startPlay();
    }

    showPlayUI();

}

void PlayerWidget::stop()
{
    d->playControlWid->setPlayState(false);

    if(d->musicPlayerMgr)
        d->musicPlayerMgr->stopPlay();

    if(d->playMonitorThread->isRunning()) {
        d->playMonitorThread->quit();
        d->playMonitorThread->wait(); //等待线程结束
    }
}

void PlayerWidget::playStateClickedSlot(bool flag)
{
    if(flag)
        d->musicPlayerMgr->keepOnPlay();
    else
        d->musicPlayerMgr->suspendPlay();
}

void PlayerWidget::obtainFileInfotmation()
{
    //get audio file information
    if(!d->audioFileInfoGet)
        d->audioFileInfoGet = new eink::AudioFileInfo(this);
    d->audioFileInfoGet->setFilePath(d->filePath.toStdString());
    d->audioFileInfoGet->startParse();

    QFileInfo file(d->filePath);
    d->fileInfo.fileName = file.baseName().toStdString();
    d->fileInfo.fileAuthor = d->audioFileInfoGet->getFileAuthor();
    d->fileInfo.timeLenth = d->audioFileInfoGet->getFileTimeLenth();
    d->fileInfo.coverImage = d->audioFileInfoGet->getFileCoverImage();

}

void PlayerWidget::showPlayUI()
{
    showFileInformation();
}

void PlayerWidget::showFileInformation()
{
    d->fileNameLab->setText(QString::fromStdString(d->fileInfo.fileName));
    QString tempStr = QString::fromLocal8Bit("歌手：") +
                      QString::fromLocal8Bit(d->fileInfo.fileAuthor.c_str());
    d->fileAuthorLab->setText(tempStr);

    //下面的控制播放按钮
    d->playControlWid->setPlayState(true);
    d->playControlWid->setFileTotleTime(d->fileInfo.timeLenth);
    d->playControlWid->setCurrentPlayTimeMillisecond(0);
    d->playControlWid->show();

    //歌词
    d->LyricWid->setBackPicture(d->fileInfo.coverImage);
    d->LyricWid->setFileName(QString::fromStdString(d->fileInfo.fileName));
    d->LyricWid->startPlayLyric();
    //d->LyricWid->show();

}

void PlayerWidget::setCurPlayTimePos(int lenth)
{
    d->playControlWid->setCurrentPlayTimeMillisecond(lenth);
    d->LyricWid->setShowTimePos(lenth);
}

void PlayerWidget::currentMusicPlayFinished()
{
    d->playControlWid->setPlayState(false);

    emit playNextMusicSig();
}

void PlayerWidget::changePlayTimePosSlot(int timePos)
{
    qDebug() << "PlayerWidget::changePlayTimePosSlot value =" << timePos;
    isMusicPlayFinished_GL = true;
    if(d->playMonitorThread->isRunning()) {
        d->playMonitorThread->quit();
        d->playMonitorThread->wait(); //等待线程结束
    }

    d->musicPlayerMgr->adjustPlayTime(timePos);

}

void PlayerWidget::signalMessageSlot(const QString & type,const QString &flag)
{
    qDebug() << "PlayerWidget::signalMessageSlot receive signal type= " << type;
    if("Eink/System" == type)
    {
        if("AdjustPlayRestart" == flag) {
            if(d->playMonitorThread->isFinished()) {
                d->playMonitorThread->start();
            }
            d->playControlWid->setPlayState(true);
        } else if("PlayFinished" == flag) {
            currentMusicPlayFinished();
        }
    }
}

