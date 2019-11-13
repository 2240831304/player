
#ifndef MP4PACKAGE_H
#define MP4PACKAGE_H

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

    class Mp4PackagePrivate;
    class Mp4Package
    {
        public:
            Mp4Package();
            ~Mp4Package();

        public:
            void setFileName(const QString &);
            void createVedioEncoder(const AVFormatContext *,const AVCodecContext *,
                            const AVRational &,int);
            void createAudioEncoder();
            void start();
            void stop();
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
            Mp4PackagePrivate *d;
    };

}
#endif