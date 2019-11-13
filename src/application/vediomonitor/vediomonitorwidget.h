
#ifndef VEDIOMONITORWIDGET_H
#define VEDIOMONITORWIDGET_H

#include <QWidget>
#include <QObject>
#include <QImage>

#include "baseapp.h"

class VedioMonitorWidgetPrivate;

class VedioMonitorWidget : public QWidget,public BaseApp
{
    Q_OBJECT
    public:
        VedioMonitorWidget(QWidget *parent=0);
        ~VedioMonitorWidget();

        virtual void ExitApplication();

    private:
        void setupUI();

    protected:
		//void paintEvent(QPaintEvent *);

    private slots:
        void startSlot();
        void stopSlot();
        void saveSlot();
        void receiveImageSlot(QImage );

    private:
        VedioMonitorWidgetPrivate *d;

};

#endif