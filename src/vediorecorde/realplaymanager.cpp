
#include "playmanager.h"
#include "sharedvariable.h"

#include <stdio.h>
#include <QDebug>
#include "vediodecoderthread.h"
#include "audiodecoderthread.h"


namespace eink {

	class PlayManagerPrivate {
		public:
			QString fileNamePath;
			AudioDecoderThread *audioPlayer;
			VedioDecoderThread *vedioPlayer;

	};

	PlayManager::PlayManager()
		: d(new PlayManagerPrivate)
	{
		init();
	}

	PlayManager::~PlayManager()
	{
        if(d->audioPlayer)
            delete d->audioPlayer;
        d->audioPlayer = NULL;

        if(d->vedioPlayer)
            delete d->vedioPlayer;
        d->vedioPlayer = NULL;
	}

	void PlayManager::init()
	{
        d->audioPlayer = new AudioDecoderThread;
        d->vedioPlayer = new VedioDecoderThread;
	}

	void PlayManager::setPlayFilePath(const QString &filePath)
	{
	    d->fileNamePath = filePath;
	}

	void PlayManager::startPlay()
	{
	    resetEvn();

	    d->audioPlayer->setFilePath(d->fileNamePath.toStdString());
	    d->audioPlayer->start();

	    d->vedioPlayer->setFilePath(d->fileNamePath.toStdString());
	    d->vedioPlayer->start();
	}

	void PlayManager::stopPlay()
	{
        d->audioPlayer->setParseState(false);
        if(d->audioPlayer->isRunning()) {
            d->audioPlayer->quit();
            d->audioPlayer->wait();
        }
        d->audioPlayer->stop();


        d->vedioPlayer->setParseState(false);
        if(d->vedioPlayer->isRunning()) {
            d->vedioPlayer->quit();
            d->vedioPlayer->wait();
        }
        d->vedioPlayer->stop();
	}

	void PlayManager::resetEvn()
	{
	    //音频，视频，数据包是否读完
        isVedioPacketReadFinished_GL = false;
        isAudioPacketReadFinished_GL = false;

        //音频，视频是否播放完
        isVedioPlayFinished_GL = false;
        isAudioDecoderFinished_GL = false;

        isFileHaveAudioStream_GL = false;

        //音频同步视频时，音频是否暂停
        isPlayAudio_GL = false;

        isCloseAudioDevice_GL = false;

        if(vedioImageQueue_GL.size() > 0)
            vedioImageQueue_GL.clear();
	}

}
