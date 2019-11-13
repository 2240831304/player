
#include "captureplaywidget.h"
#include <QPainter>
#include <QDebug>
#include "sharedvariable.h"

CapturePlayWidget::CapturePlayWidget(QWidget *parent)
    : QWidget(parent)
{
}


CapturePlayWidget::~CapturePlayWidget()
{
}

void CapturePlayWidget::setImage(QImage picture)
{
    image = picture;

    update();
}

void CapturePlayWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setBrush(Qt::SolidPattern);
    painter.drawRect(0,0,width(),height());

    if(image.isNull()) {
        qDebug() << "CapturePlayWidget::paintEvent image is null!!!";
        return;
    } else {
        //计算从哪开始绘制图像
        int x = this->width() - image.width();
        int y = this->height() - image.height();

        x /= 2;
        y /= 2;

        painter.drawImage(QPoint(x,y),image); //画出图像
    }
}

void CapturePlayWidget::resizeEvent(QResizeEvent *)
{
    desktopGrabPlayWidgetWidth_GL = this->width();
    desktopGrabPlayWidgetHeight_GL = this->height();
}