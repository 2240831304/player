

#include "audiodecoderthread.h"

#include "audiodecodermp4.h"
#include <QDebug>
#include <QFileInfo>

namespace eink{

    AudioDecoderThread::AudioDecoderThread(QObject *parent)
        : QThread(parent)
    {
        InitEvn();
    }

    AudioDecoderThread::~AudioDecoderThread()
    {
        audioDecoder = NULL;
    }

    void AudioDecoderThread::setFilePath(const std::string &path)
    {
        qDebug() << "vedio file decde audion AudioDecoderThread::setFilePath!!";
        filePath = path;
    }

    void AudioDecoderThread::run()
    {
        audioDecoder = NULL;

        QFileInfo file(QString::fromStdString(filePath));
	    QString suffix = file.suffix();

	    audioDecoder = audioPlayTypeMap[suffix.toStdString()];

	    if(audioDecoder) {
	        audioDecoder->setFilePath(filePath);
	        audioDecoder->play();
	    } else {
	        qDebug() << "AudioDecoderThread::run() bot find audio palyer!!!!!";
	    }
    }

    void AudioDecoderThread::InitEvn()
    {
        AudioDecoder *audioDecoderMp4 = new AudioDecoderMp4;
        audioPlayTypeMap[audioDecoderMp4->getVedioPlayerType()] = audioDecoderMp4;
        audioDecoder = NULL;
    }

    void AudioDecoderThread::play()
    {
    }

    void AudioDecoderThread::stop()
    {
        if(audioDecoder)
            audioDecoder->stop();
    }

    void AudioDecoderThread::suspendPlay()
    {
    }

    void AudioDecoderThread::keepOnPlay()
    {
    }

    void AudioDecoderThread::adjustPlayTiemPos(int )
    {
    }

    void AudioDecoderThread::setParseState(bool flag)
    {
        if(audioDecoder)
            audioDecoder->setParseState(flag);
    }

}
