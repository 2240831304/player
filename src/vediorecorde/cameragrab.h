
/**************
摄像头监控
**************/


#ifndef CAMERAGRAB_H
#define CAMERAGRAB_H
#include <QImage>
#include <QObject>
#include "basegrab.h"
#include "sdlplaywidget.h"

namespace eink {
    class CameraGrabPrivate;

    class CameraGrab : public QObject,public BaseGrab
    {
        Q_OBJECT
        public:
            CameraGrab(QObject *parent =0);
            ~CameraGrab();

            virtual void start();
            virtual void saveFile();
            virtual void stop();

        public:
            void startWork();
            void setWorkingState(bool );
            void setSdlPlayWidget(eink::SdlPlayWidget *);

        private:
            void InitEvn();
            void fillFormatInfo();
            void findStream();
            void readFrameData();
            void freeMemory();

        signals:
            void sendImageSig(QImage );

        private:
            CameraGrabPrivate *d;
    };

}

#endif