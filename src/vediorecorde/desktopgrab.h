
/**************
桌面抓频
**************/


#ifndef DESKTOPGRAB_H
#define DESKTOPGRAB_H
#include <QImage>
#include <QObject>
#include "basegrab.h"
#include "sdlplaywidget.h"

namespace eink {
    class DesktopGrabPrivate;

    class DesktopGrab : public QObject,public BaseGrab
    {
        Q_OBJECT
        public:
            DesktopGrab(QObject *parent =0);
            ~DesktopGrab();

            virtual void start();
            virtual void saveFile();
            virtual void stop();

        public:
            void startWork();
            void setWorkingState(bool );
            void rtmpFlugVedio();
            void setSdlPlayWidget(eink::SdlPlayWidget *);

        private:
            void InitEvn();
            void fillFormatInfo();
            void findStream();
            void readFrameData();
            void freeMemory();

        private:
            DesktopGrabPrivate *d;
    };

}

#endif