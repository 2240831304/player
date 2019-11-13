
#ifndef TABBAR_H
#define TABBAR_H

#include <QObject>
#include <QTabBar>
#include <QTabWidget>

namespace eink {
    class TabBar : public QTabBar
    {
        Q_OBJECT
        public:
            TabBar(QWidget *parent =0);
            ~TabBar();

        protected:
            //void paintEvent(QPaintEvent * );
    };

    class TabWidget : public QTabWidget
    {
        Q_OBJECT
        public:
            TabWidget(QWidget *parent =0);
            ~TabWidget();

            void setWidgetTabBar(TabBar *);

    };

}

#endif