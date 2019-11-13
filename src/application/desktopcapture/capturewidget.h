
#ifndef CAPTUREWIDGET_H
#define CAPTUREWIDGET_H

#include <QWidget>
#include <QObject>
#include <QImage>

#include "baseapp.h"

class CaptureWidgetPrivate;

class CaptureWidget : public QWidget,public BaseApp
{
    Q_OBJECT
    public:
        CaptureWidget(QWidget *parent=0);
        ~CaptureWidget();

        virtual void ExitApplication();

    private:
        void setupUI();

    protected:
		//void paintEvent(QPaintEvent *);

    private slots:
        void startSlot();
        void stopSlot();
        void saveSlot();
        void rtmpSlot();
        void receiveImageSlot(QImage );

    private:
        CaptureWidgetPrivate *d;

};

#endif