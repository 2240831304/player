
#include "showpicturewidget.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QApplication>
#include <QDesktopWidget>
#include <QPainter>

#include <stdio.h>
#include <QDebug>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/highgui/highgui_c.h>
using namespace cv;


namespace eink {

	class ShowPictureWidgetPrivate {
		public:
            QString lastPlayFile;
            ShowPicturePlayWidget *drawImageWidget;
            QWidget *testWID;

	};



	ShowPictureWidget::ShowPictureWidget(QWidget *parent)
		:QWidget(parent),d(new ShowPictureWidgetPrivate)
	{
		setWindowFlags(Qt::FramelessWindowHint);//无边框

		setupFace();
	}


	ShowPictureWidget::~ShowPictureWidget()
	{
		if(d)
			delete d;
		d = NULL;
	}

	void ShowPictureWidget::setupFace()
	{
        //d->drawImageWidget = new ShowPicturePlayWidget(this);
        //namedWindow("test");
        //d->testWID = (QWidget*)cvGetWindowHandle("test");

        QVBoxLayout *mainVlayout = new QVBoxLayout(this);
        //mainVlayout->addWidget(d->testWID);
        setLayout(mainVlayout);
	}

	void ShowPictureWidget::resetEvn()
	{
	}

    void ShowPictureWidget::setFilePath(const QString &path)
    {
        //Mat img = imread(path.toStdString().c_str());
        //imshow("test",img);
        //QWidget* cvWidget = (QWidget*)cvGetWindowHandle("游戏原图");
    }



	ShowPicturePlayWidget::ShowPicturePlayWidget(QWidget *parent)
		: QWidget(parent)
	{

	}

	ShowPicturePlayWidget::~ShowPicturePlayWidget()
	{
	}

	void ShowPicturePlayWidget::paintEvent(QPaintEvent *)
	{
//		QPainter painter(this);
//		painter.setBrush(Qt::black);
//		painter.drawRect(0, 0, this->width(), this->height()); //先画成黑色
	}

	void ShowPicturePlayWidget::resizeEvent(QResizeEvent *event)
	{
//	    qDebug() << "ShowPicturePlayWidget::resizeEvent get playUI size!!!"
//	            << "width=" << width() << "height=" << height();
//	    playUIWidth_GL = width();
//	    playUIHeight_GL = height();
	}

}
