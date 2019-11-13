
#ifndef RtmpliveWidget_H
#define RtmpliveWidget_H

#include <QWidget>
#include <QObject>
#include <QImage>

#include "baseapp.h"

class RtmpliveWidgetPrivate;

class RtmpliveWidget : public QWidget,public BaseApp
{
    Q_OBJECT
    public:
        RtmpliveWidget(QWidget *parent=0);
        ~RtmpliveWidget();

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
        RtmpliveWidgetPrivate *d;

};

#endif
