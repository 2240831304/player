
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

namespace eink {

	class PlayWidgetPrivate {
		public:

			DrawImageWidget *drawImageWidget;
			int timerID;
			int timerLenth;  //多少毫秒播放一张图片
			bool playState;
			QMutex mutex;
			QTimer *timer;

	};

	PlayWidget::PlayWidget(QWidget *parent)
		:QWidget(parent)
		 ,d(new PlayWidgetPrivate)
	{
		setWindowFlags(Qt::FramelessWindowHint);//无边框
		d->playState = false;

		d->timerLenth = 41;

		d->timer = new QTimer(this);
		connect(d->timer,SIGNAL(timeout()),this,SLOT(timerSlot()));

		setupFace();
	}


	PlayWidget::~PlayWidget()
	{
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

		QHBoxLayout *baseOperateLayout = baseOperate();

		QVBoxLayout *layoutMian = new QVBoxLayout(this);
		layoutMian->setContentsMargins(0,0,0,0);
		layoutMian->addWidget(d->drawImageWidget,10);
		layoutMian->addLayout(baseOperateLayout,1);
		setLayout(layoutMian);
	}

	QHBoxLayout *PlayWidget::baseOperate()
	{
		QPushButton *stopPlayButton = new QPushButton(QString::fromLocal8Bit("暂停"),this);
		connect(stopPlayButton,SIGNAL(clicked()),this,SLOT(stopPlaySot()));

		QPushButton *continuePlayButton = new QPushButton(QString::fromLocal8Bit("继续"),this);
		connect(continuePlayButton,SIGNAL(clicked()),this,SLOT(continuePlaySlot()));

		QHBoxLayout *hlayout1 = new QHBoxLayout();
		hlayout1->addWidget(stopPlayButton);
		hlayout1->addWidget(continuePlayButton);

		return hlayout1;
	}

	void PlayWidget::stopPlaySot()
	{
	    //循环定时器停止
	    if(d->playState)
	        killTimer(d->timerID);

	    d->playState = false;
	    isPlayAudio_GL = false;
	}

	void PlayWidget::restartPlay()
	{
	    //循环定时器停止
	    if(d->playState)
	        killTimer(d->timerID);

	    d->playState = false;
	    isPlayAudio_GL = false;
	    isCloseAudioDevice_GL = true;
	}

	bool PlayWidget::getPlayState()
	{
	    return d->playState;
	}

	void PlayWidget::continuePlaySlot()
	{
	    d->playState = true;
	    d->timerID = startTimer(d->timerLenth);
	}

	void PlayWidget::startPlay()
	{
        d->timer->start(1000);
	}

	void PlayWidget::timerEvent(QTimerEvent *event)
	{
//	    static QDateTime startTime = QDateTime::currentDateTime();
//	    QDateTime cur = QDateTime::currentDateTime();
//	    qDebug() << "22222222222223###########=======" << startTime.msecsTo(cur);

        if(!d->playState) {
            isPlayAudio_GL = false;
            return;
        }

        if(vedioImageQueue_GL.size() < 10) {
            if(!isVedioPacketReadFinished_GL){
                isPlayAudio_GL = false;
                killTimer(d->timerID);
                d->timer->start(1000);
                return;
            }
        }

        if(vedioImageQueue_GL.empty()) {
            if(isVedioPacketReadFinished_GL) {
                isPlayAudio_GL = false;
                killTimer(d->timerID);
                playFinished();
                return;
            } else {
                isPlayAudio_GL = false;
                killTimer(d->timerID);
                d->timer->start(1000);
                return;
            }
        }

        //音频播放完毕，视频停止播放
        if(isAudioDecoderFinished_GL){
            isPlayAudio_GL = false;
            killTimer(d->timerID);
            return;
        }

	    if(event->timerId() == d->timerID ) {
	        isPlayAudio_GL = true;

	        Vedio_Image_ST temp = vedioImageQueue_GL.front();
	        qDebug() << "PlayWidget::timerEvent curAudioDecoderTimePos_GL=" << curAudioDecoderTimePos_GL
	                << "temp.playTimePos=" << temp.playTimePos;
	        if(curAudioDecoderTimePos_GL >= temp.playTimePos) {
                QImage  image = temp.image;
                d->mutex.lock();
                curVedioPlayTimePos_GL = temp.playTimePos;
                d->mutex.unlock();

                d->drawImageWidget->setImage(image);
                vedioImageQueue_GL.pop_front();
	        }
	    }

    }

    void PlayWidget::timerSlot()
    {
        qDebug() << "PlayWidget::timerSlot() prepare play!!";
        d->timer->stop();
        //如果缓存不足100，视频数据包没有解析完，继续缓存
        if( (vedioImageQueue_GL.size() > 100) || isVedioPacketReadFinished_GL) {
            if(frameRate_GL != 0)
                d->timerLenth = frameRate_GL;
            //调用object::startTimer()定时器函数，循环定时器
            qDebug() << "PlayWidget::timerSlot() frame rate d->timerLenth = " << d->timerLenth;
            d->playState = true;
	        d->timerID = startTimer(d->timerLenth);
        } else {
            d->timer->start(1000);
        }
    }

    void PlayWidget::playFinished()
    {
        isVedioPlayFinished_GL = true;
        d->playState = false;
    }




	DrawImageWidget::DrawImageWidget(QWidget *parent)
		: QWidget(parent),drawImageNum(0)
	{
	    isPlayFinished = false;
	}

	DrawImageWidget::~DrawImageWidget()
	{
	}

	void DrawImageWidget::setImage(const QImage &pic)
	{
		mImage = pic;
		++drawImageNum;

		update();
	}

	void DrawImageWidget::paintEvent(QPaintEvent *)
	{
		QPainter painter(this);
		painter.setBrush(Qt::black);
		painter.drawRect(0, 0, this->width(), this->height()); //先画成黑色

		if(mImage.isNull())
			return;

		if (mImage.size().width() <= 0)
			return;

        //计算从哪开始绘制图像
		int x = this->width() - mImage.width();
		int y = this->height() - mImage.height();

		x /= 2;
		y /= 2;

		painter.drawImage(QPoint(x,y),mImage); //画出图像

		//画出时间
		int time = drawImageNum * frameRate_GL;
		QRect rect = QRect(20,height()-30,100,30);
		painter.drawText(rect, QString::number(time), QTextOption(Qt::AlignCenter));
	}

	void DrawImageWidget::resizeEvent(QResizeEvent *event)
	{
	    qDebug() << "DrawImageWidget::resizeEvent get playUI size!!!"
	            << "width=" << width() << "height=" << height();
	    playUIWidth_GL = width();
	    playUIHeight_GL = height();
	}

	void DrawImageWidget::setPlayFinishedState(bool flag)
	{
	    isPlayFinished = flag;
	}

}
