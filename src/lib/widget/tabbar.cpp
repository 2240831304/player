
#include "tabbar.h"
#include <QPainter>

namespace eink{

    TabBar::TabBar(QWidget *parent)
        : QTabBar(parent)
    {
    }

    TabBar::~TabBar()
    {
    }

//    void TabBar::paintEvent(QPaintEvent * )
//    {
//        QPainter painter(this);
//		painter.setBrush(Qt::black);
//
//		painter.drawRect(0, 0, this->width(), this->height());
//    }


    TabWidget::TabWidget(QWidget *parent )
        : QTabWidget(parent)
    {
    }

    TabWidget::~TabWidget()
    {
    }

    void TabWidget::setWidgetTabBar(TabBar *bar)
    {
        setTabBar(bar);
    }
}