
#ifndef REALTIMEVEDIOPLAY_H
#define REALTIMEVEDIOPLAY_H
#include <QImage>
#include <QObject>
#include <QThread>
#include "sdlplaywidget.h"
#include "rtmpplugflow.h"

extern "C"{
    #include <libavcodec/avcodec.h>
    #include <libavformat/avformat.h>
    #include <libavutil/pixfmt.h>
    #include <libswscale/swscale.h>
    #include <libavutil/fifo.h>
    #include <libavdevice/avdevice.h>
    #include <libavutil/mem.h>
    #include <SDL2/SDL.h>
}

namespace eink {
    class RealTimeVedioPlayPrivate;

    class RealTimeVedioPlay : public QThread
    {
        Q_OBJECT
        public:
            RealTimeVedioPlay(QObject *parent = 0);
            ~RealTimeVedioPlay();

            void saveFile();
            void stop();

        public:
            void InitEvn();
            void startWork();
            void setFormatCtx( AVFormatContext **ctx);
            void setWorkingState(bool );
            void setSdlPlayWidget(eink::SdlPlayWidget *);
            void decodeVedioPacket( AVPacket *dataPacket);
            void setRtmpPusher(RtmpPlugFlow *);
            void startRtmpPushVedio();

        private:
            void findStream();
            void setConvertFormat();
            void parseFrameToImage();
            void freeMemory();
            void parseVedioData();

            void setFileName();

            void tramslationToYuvSdlPlay();
            void setTramslationToYuvSdlInfo();
            void setTransToRgbInfo();

        protected:
            void run();

        signals:
            void sendImageSig(QImage );

        private:
            RealTimeVedioPlayPrivate *d;
    };

}

#endif
