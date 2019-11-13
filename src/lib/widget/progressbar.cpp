
#include "progressbar.h"
#include <QDebug>
#include <QCoreApplication>
    
namespace eink{

    ProgressBar::ProgressBar(QWidget *parent )
        : QSlider(parent),isMousePress(false)
    {
        setOrientation(Qt::Horizontal);
        installEventFilter(this);
    }

    ProgressBar::~ProgressBar()
    {
    }
     
    void ProgressBar::mousePressEvent(QMouseEvent *ev)
    {
        //注意应先调用父类的鼠标点击处理事件，这样可以不影响拖动的情况
        //QSlider::mousePressEvent(ev);
        //获取鼠标的位置，这里并不能直接从ev中取值（因为如果是拖动的话，鼠标开始点击的位置没有意义了）
        //double pos = ev->pos().x() / (double)width();
        //setValue(pos * (maximum() - minimum()) + minimum());
        //向父窗口发送自定义事件event type，这样就可以在父窗口中捕获这个事件进行处理
        //QEvent evEvent(static_cast<QEvent::Type>(QEvent::User) + 1);
        //QCoreApplication::sendEvent(parentWidget(), &evEvent);

        //int curValue = sliderPosition();
        //qDebug() << "55555555555555=====width()=======" << width() << curValue;
    }

    void ProgressBar::mouseReleaseEvent(QMouseEvent *ev)
    {
        //qDebug() << "666666666====width()=======" << width() << sliderPosition();

        int curValue = sliderPosition();
        setValue(curValue);

        emit changePlayTimePos(curValue);
        //QSlider::mouseReleaseEvent(ev);
    }

    void ProgressBar::mouseMoveEvent ( QMouseEvent * ev)
    {
        double pos = ev->pos().x() / (double)width();
        setValue(pos * (maximum() - minimum()) + minimum());
    }

    bool ProgressBar::eventFilter(QObject *obj, QEvent *event)
    {
        if(obj == this)
        {
            if (event->type() == QEvent::MouseButtonPress)
            {
                QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
                if (mouseEvent->button() == Qt::LeftButton) //判断左键
                {
                   int dur = maximum() - minimum();
                   int pos = minimum() + dur * ((double)mouseEvent->x() / width());
                   if(pos != sliderPosition())
                   {
                      setValue(pos);
                   }
                }
            }
        }
        return QObject::eventFilter(obj,event);
    }


}