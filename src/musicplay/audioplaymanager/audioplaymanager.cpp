
#include "audioplaymanager.h"
#include <stdio.h>
#include <QDebug>
#include <QFileInfo>
#include "audiofiledecoder.h"
#include "audiofiledecodermp4.h"
#include "audiofileplaythread.h"
#include "audiofiledecoderwav.h"
#include "audiofiledecodermp3.h"
#include "audiofiledecoderflac.h"
#include "sharedvariable.h"


namespace eink {

	class AudioPlayManagerPrivate {
		public:
		    AudioPlayManagerPrivate()
		    {
		        player = NULL;
		    }

            QString fileNamePath;

			std::map<std::string,AudioFileDecoder *> playerMap;
			AudioFileDecoder *player;
			AudioFilePlayThread *playerThread;

	};

	AudioPlayManager::AudioPlayManager()
		: d(new AudioPlayManagerPrivate)
	{
		init();
	}

	AudioPlayManager::~AudioPlayManager()
	{
		std::map<std::string ,AudioFileDecoder *>::iterator it;
		for(it = d->playerMap.begin();it != d->playerMap.end();it++)
			delete (*it).second;
		d->playerMap.clear();
		qDebug() << "AudioPlayManager::~AudioPlayManager()";

		if(d->playerThread)
		    delete d->playerThread;
		d->playerThread = NULL;

		if(d)
			delete d;

	}

	void AudioPlayManager::init()
	{
        qDebug() << "AudioPlayManager::init() each file type to play" ;

        //.mp4
		AudioFileDecoder *mp4Player = new eink::AudioFileDecoderMp4;
		if(mp4Player) {
			d->playerMap[mp4Player->getAudioPlayerType()] = mp4Player;
		}

        //.mp3
		AudioFileDecoder *mp3Player = new eink::AudioFileDecoderMp3;
		if(mp3Player) {
			d->playerMap[mp3Player->getAudioPlayerType()] = mp3Player;
		}

        //.wav
        AudioFileDecoder *wavPlayer = new AudioFileDecoderWav;
        if(wavPlayer){
            d->playerMap[wavPlayer->getAudioPlayerType()] = wavPlayer;
        }

        //.flac
        AudioFileDecoder *flacPlayer = new AudiofileDecoderFlac;
        if(flacPlayer){
            d->playerMap[flacPlayer->getAudioPlayerType()] = flacPlayer;
        }

        d->playerThread = new AudioFilePlayThread;
	}

	void AudioPlayManager::setPlayFilePath(const QString &filePath)
	{
	    d->fileNamePath = filePath;
	}

	void AudioPlayManager::startPlay()
	{
	    d->player = NULL;
	    resetEvn();

	    QFileInfo file(d->fileNamePath);
	    QString suffix = file.suffix();

	    d->player = d->playerMap[suffix.toStdString()];
	    if(d->player) {
	        d->player->setFilePath(d->fileNamePath.toStdString());
	        d->playerThread->setAudioFilePlayer(d->player);
	        d->playerThread->start();
	    } else {
	        qDebug() << "AudioPlayManager::startPlay() Not find player to play this file!!";
	    }

	}

	void AudioPlayManager::stopPlay()
	{
	    qDebug() << "AudioPlayManager::stopPlay()~!!!!!!!!!!";
	    resetEvn();
	    d->player->stop();
	    d->player = NULL;
	}

	void AudioPlayManager::resetEvn()
	{
        if(d->playerThread->isRunning()) {
            qDebug() << "d->playerThread->isRunning()";
            d->playerThread->quit();
            d->playerThread->wait(); //等待线程结束
        }

        isMusicStartPlay_GL = false;
        isMusicPlayFinished_GL = false;
        musicPlayTimePos_GL = 0;
        isAutoMusicPlayFinished_GL = false;
	}

	void AudioPlayManager::suspendPlay()
	{
	    d->player->suspendPlay();
	}

	void AudioPlayManager::keepOnPlay()
	{
	    d->player->keepOnPlay();
	}

	void AudioPlayManager::adjustPlayTime(int posTime)
	{
	    d->player->adjustPlayTimePos(posTime);
	}


}
