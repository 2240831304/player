

#include "vediodecoderthread.h"
#include <QDebug>
#include "vediodecodermp4.h"
#include <QFileInfo>

namespace eink {

    VedioDecoderThread::VedioDecoderThread(QObject *parent)
        : QThread(parent)
    {
        InitEvn();
    }

    VedioDecoderThread::~VedioDecoderThread()
    {
    }

    void VedioDecoderThread::run()
    {
        curVedioDecoder = NULL;

        QFileInfo file(QString::fromStdString(fileDir));
	    QString suffix = file.suffix();

	    curVedioDecoder = vedioDecoderMap[suffix.toStdString()];

	    if(curVedioDecoder) {
	        curVedioDecoder->setFilePath(fileDir);
	        curVedioDecoder->play();
	    } else {
	        qDebug() << "VedioDecoderThread::run() bot find audio palyer!!!!!";
	    }
    }

    void VedioDecoderThread::InitEvn()
    {
        curVedioDecoder = NULL;

        VedioDecoder *vedioDecoderMp4 = new VedioDecoderMp4;
        vedioDecoderMap[vedioDecoderMp4->getVedioPlayerType()] = vedioDecoderMp4;
    }

    void VedioDecoderThread::play()
    {
    }

    void VedioDecoderThread::stop()
    {
        if(curVedioDecoder)
            curVedioDecoder->stop();
    }

    void VedioDecoderThread::suspendPlay()
    {
    }

    void VedioDecoderThread::keepOnPlay()
    {
    }

    void VedioDecoderThread::adjustPlayTiemPos(int )
    {
    }

    void VedioDecoderThread::setFilePath(const std::string &path)
    {
        fileDir = path;
    }

    void VedioDecoderThread::setParseState(bool flag)
    {
       if(curVedioDecoder)
            curVedioDecoder->setParseState(flag);
    }


}

