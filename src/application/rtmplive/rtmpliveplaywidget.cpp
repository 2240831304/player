
#include "rtmpliveplaywidget.h"
#include <QPainter>
#include <QDebug>
#include "sharedvariable.h"

RtmplivePlayWidget::RtmplivePlayWidget(QWidget *parent)
    : QWidget(parent)
{
}


RtmplivePlayWidget::~RtmplivePlayWidget()
{
}

void RtmplivePlayWidget::setImage(QImage picture)
{
    image = picture;

    update();
}

void RtmplivePlayWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setBrush(Qt::SolidPattern);
    //painter.setBrush(Qt::white);
    painter.drawRect(0,0,width(),height());

    if(image.isNull()) {
        qDebug() << "RtmplivePlayWidget::paintEvent image is null!!!";
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

void RtmplivePlayWidget::resizeEvent(QResizeEvent *)
{
    desktopGrabPlayWidgetWidth_GL = this->width();
    desktopGrabPlayWidgetHeight_GL = this->height();
}