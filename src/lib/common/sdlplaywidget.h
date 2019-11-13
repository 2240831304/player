#ifndef SDLPLAYWIDGET_H
#define SDLPLAYWIDGET_H

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

    class SdlPlayWidgetPrivate;
    class SdlPlayWidget {

        public:
            SdlPlayWidget();
            ~SdlPlayWidget();

            void setSdlUISize(int,int);
            void createSdlVedioPlayWidget(void *pWnd = NULL);
            void InitWindow();
            void playVedioData(AVFrame *pFrameYUV);

        private:
            void clearEvn();
            void exitPLayVedio();

        private:
            SdlPlayWidgetPrivate *d;

    };

}

#endif
