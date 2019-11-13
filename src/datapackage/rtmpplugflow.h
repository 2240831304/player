
#ifndef RTMPPLUGFLOW_H
#define RTMPPLUGFLOW_H

#include <QString>

extern "C"{
    #include <libavcodec/avcodec.h>
    #include <libavformat/avformat.h>
    #include <libavutil/pixfmt.h>
    #include <libswscale/swscale.h>
	#include <libavutil/fifo.h>
	#include <libavdevice/avdevice.h>
	#include <libavutil/mem.h>
}

#include <iostream>

namespace eink{

    class RtmpPlugFlowPrivate;
    class RtmpPlugFlow
    {
        public:
            RtmpPlugFlow();
            ~RtmpPlugFlow();

        public:
            void setFileName(const QString &);
            bool createVedioEncoder(const AVFormatContext *,const AVCodecContext *,
                            const AVRational &,int);
            void createAudioEncoder();
            void start();
            void stop();
            bool openPushAddress();
            void fillVedioData(const AVFrame *);
            void fillAudioData(const AVFrame *);


        private:
            void InitEvn();
            void freeMemory();
            void writeHeader();
            void writeTrailer();
            void setScaleInfo(const AVFormatContext *,const AVCodecContext *);
            void setResampleInfo();


        private:
            RtmpPlugFlowPrivate *d;
    };

}
#endif