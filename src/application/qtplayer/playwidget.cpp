
#include "playwidget.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QApplication>
#include <QDesktopWidget>
#include <QPainter>
#include <QTimerEvent>

#include <stdio.h>
#include <QDebug>
#include "sharedvariable.h"
#include <QMutex>
#include <QTimer>
#include "struct.h"
#include <QDateTime>
#include "playmanager.h"
#include "playcontrolwidget.h"


namespace eink {

	class PlayWidgetPrivate {
		public:

			DrawImageWidget *drawImageWidget;
			int timerID;
			int timerLenth;  //多少毫秒播放一张图片
			bool playState;
			QMutex mutex;
			QTimer *timer;
			PlayManager *playerMgr;
            QString filePath;
            PlayControlWidget *playControlWid;
            bool isPlaySuspendState;
            QString lastPlayFile;
	};

	PlayWidget::PlayWidget(QWidget *parent)
		:QWidget(parent),d(new PlayWidgetPrivate)
	{
		setWindowFlags(Qt::FramelessWindowHint);//无边框

		d->timer = new QTimer(this);
		connect(d->timer,SIGNAL(timeout()),this,SLOT(timerSlot()));

		resetEvn();

		setupFace();
	}


	PlayWidget::~PlayWidget()
	{
	    if(d->playerMgr)
	        delete d->playerMgr;
	    d->playerMgr = NULL;

		if(d) {
			delete d;
			d = NULL;
		}
	}

	void PlayWidget::setupFace()
	{
		d->drawImageWidget = new DrawImageWidget(this);
		//d->playWidget->setFixedSize(qApp->desktop()->width(),qApp->desktop()->height()*5/6);
		d->drawImageWidget->setStyleSheet(QString::fromUtf8("border:1px solid red"));

		d->playControlWid = new PlayControlWidget(this);
		d->playControlWid->setPlayState(false);
		connect(d->playControlWid,SIGNAL(playStateClickedSig(bool)),
                this,SLOT(playStateClickedSlot(bool)));
        //connect(d->playControlWid,SIGNAL(nextPlaySig()),SIGNAL(playNextMusicSig()));
        //connect(d->playControlWid,SIGNAL(lastPlaySig()),SIGNAL(playLastMusicSig()));
        //connect(d->playControlWid,SIGNAL(changePlayTimePos(int )),
        //        this,SLOT(changePlayTimePosSlot(int )));

		QVBoxLayout *layoutMian = new QVBoxLayout(this);
		layoutMian->setContentsMargins(0,0,0,0);
		layoutMian->addWidget(d->drawImageWidget,10);
		layoutMian->addWidget(d->playControlWid,1);
		setLayout(layoutMian);

		d->playerMgr = new  PlayManager;
	}

	void PlayWidget::resetEvn()
	{
	    d->playState = false;
		d->timerLenth = 41;
		d->isPlaySuspendState = false;
	}

	void PlayWidget::playStateClickedSlot(bool flag)
	{
	    if(isVedioPlayFinished_GL)
	        return;

	    if(flag) {
	        continuePlay();
	    } else {
	        suspendPlay();
	    }
	}

	void PlayWidget::suspendPlay()
	{
	    //循环定时器停止
	    if(d->playState)
	        killTimer(d->timerID);

            d->isPlaySuspendState = true;
	    d->playState = false;
	    isPlayAudio_GL = false;
	}

	void PlayWidget::continuePlay()
	{
	    d->playState = true;
	    d->isPlaySuspendState = false;
	    d->timerID = startTimer(d->timerLenth);
            d->playControlWid->setPlayState(true);
	}

	bool PlayWidget::getPlayState()
	{
	    return d->playState;
	}

	void PlayWidget::startPlay()
    {
        if(d->lastPlayFile != d->filePath) {
            stopPlay();
        } else {
            if(d->playState || d->isPlaySuspendState)
                return;
        }

	    resetEvn();

        d->lastPlayFile = d->filePath;
	    d->playerMgr->setPlayFilePath(d->filePath);
	    d->playerMgr->startPlay();

        d->timer->start(500);
	}

	void PlayWidget::stopPlay()
    {
        //循环定时器停止
	    if(d->playState)
	        killTimer(d->timerID);

	    d->playState = false;
	    isPlayAudio_GL = false;

	    d->playerMgr->stopPlay();
    }

    void PlayWidget::timerSlot()
    {
        qDebug() << "PlayWidget::timerSlot() prepare play!!";
        d->timer->stop();
        //如果缓存不足100，视频数据包没有解析完，继续缓存
        if( (vedioImageQueue_GL.size() > 100) || isVedioPacketReadFinished_GL) {
            if(frameRate_GL != 0)
                d->timerLenth = frameRate_GL;
            qDebug() << "PlayWidget::timerSlot() frame rate d->timerLenth = " << d->timerLenth;
            //调用object::startTimer()定时器函数，循环定时器
            d->playState = true;
	        d->timerID = startTimer(d->timerLenth);
            d->playControlWid->setPlayState(true);
            d->playControlWid->setFileTotleTime(vedioTotleTimeLenth_GL);
        } else {
            d->timer->start(500);
        }
    }

	void PlayWidget::timerEvent(QTimerEvent *event)
	{
        if(!d->playState) {
            isPlayAudio_GL = false;
            qDebug() << "PlayWidget::timerEvent current not play!!";
            return;
        }

        //
        if(vedioImageQueue_GL.size() < 10) {
            if(!isVedioPacketReadFinished_GL){
                isPlayAudio_GL = false;
                killTimer(d->timerID);
                d->timer->start(500);
                return;
            }
        }

        if(vedioImageQueue_GL.empty()) {
            isPlayAudio_GL = false;
            killTimer(d->timerID);
            if(isVedioPacketReadFinished_GL) {
                playFinished();
            } else {
                d->timer->start(500);
            }
            return;
        }

        //音频播放完毕，视频停止播放
        if(isAudioDecoderFinished_GL){
            isPlayAudio_GL = false;
            killTimer(d->timerID);
            return;
        }

	    if(event->timerId() == d->timerID ) {
            isPlayAudio_GL = true;
            d->playControlWid->setCurrentPlayTimeMillisecond(curAudioDecoderTimePos_GL);

            d->mutex.lock();
	        Vedio_Image_ST temp = vedioImageQueue_GL.front();
	        d->mutex.unlock();

	        if(!isFileHaveAudioStream_GL)
	            d->playControlWid->setCurrentPlayTimeMillisecond(temp.playTimePos);
            //qDebug() << "PlayWidget::timerEvent curAudioDecoderTimePos_GL=" << curAudioDecoderTimePos_GL
            //            << "temp.playTimePos=" << temp.playTimePos << d->timerLenth;
	        if( (curAudioDecoderTimePos_GL >= temp.playTimePos) ||
	                    !isFileHaveAudioStream_GL ) {
                d->drawImageWidget->setImage(temp.image);
                d->mutex.lock();
                vedioImageQueue_GL.pop_front();
                d->mutex.unlock();
	        }
	    }

    }

    void PlayWidget::playFinished()
    {
        d->playControlWid->setPlayState(false);
        isVedioPlayFinished_GL = true;
        isPlayAudio_GL = false;
        d->playState = false;
    }

    void PlayWidget::setFilePath(const QString &path)
    {
        d->filePath = path;
    }




	DrawImageWidget::DrawImageWidget(QWidget *parent)
		: QWidget(parent)
	{
        //setAttribute(Qt::WA_OpaquePaintEvent);
	}

	DrawImageWidget::~DrawImageWidget()
	{
	}

	void DrawImageWidget::setImage(QImage pic)
	{
		mImage = pic;

		update();
	}

	void DrawImageWidget::paintEvent(QPaintEvent *)
	{
		QPainter painter(this);
		painter.setBrush(Qt::black);
		painter.drawRect(0, 0, this->width(), this->height()); //先画成黑色

		if(mImage.isNull()) {
		    qDebug() << "DrawImageWidget::paintEvent image is null!!!";
			return;
	    } else {
            //计算从哪开始绘制图像
            int x = this->width() - mImage.width();
            int y = this->height() - mImage.height();

            x /= 2;
            y /= 2;

            painter.drawImage(QPoint(x,y),mImage); //画出图像
		}

		//画出时间
		//int time = drawImageNum * frameRate_GL;
		//QRect rect = QRect(20,height()-30,100,30);
		//painter.drawText(rect, QString::number(time), QTextOption(Qt::AlignCenter));
	}

	void DrawImageWidget::resizeEvent(QResizeEvent *event)
	{
	    qDebug() << "DrawImageWidget::resizeEvent get playUI size!!!"
	            << "width=" << width() << "height=" << height();
	    playUIWidth_GL = width();
	    playUIHeight_GL = height();
	}

}
