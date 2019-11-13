
/**************
桌面抓频
**************/


#ifndef LOVELIVE_H
#define LOVELIVE_H
#include <QImage>
#include <QObject>
#include "basegrab.h"
#include "sdlplaywidget.h"

namespace eink {
    class LoveLivePrivate;

    class LoveLive : public QObject,public BaseGrab
    {
        Q_OBJECT
        public:
            LoveLive(QObject *parent =0);
            ~LoveLive();

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
            void createCodec();
            void readFrameData();
            void readFrameDataLocalParse();
            void parseFrameToImage();
            void freeMemory();

            void setFileName();

            void tramslationToSdlDataPlay();
            void setTramslationToSdlInfo();

        signals:
            void sendImageSig(QImage );

        private:
            LoveLivePrivate *d;
    };

}

#endif
