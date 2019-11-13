

#include "audiofileplaythread.h"
#include <QDebug>

AudioFilePlayThread::AudioFilePlayThread(QObject *parent)
    : QThread(parent)
{
}

AudioFilePlayThread::~AudioFilePlayThread()
{
}


void AudioFilePlayThread::run()
{
    audioPlayer->play();
}

void AudioFilePlayThread::setAudioFilePlayer( AudioFileDecoder *player)
{
    audioPlayer = player;
}

