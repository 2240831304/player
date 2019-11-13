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
    #include <SDL2/SDL_image.h>
}

namespace eink {

    class SdlPlayWidgetPrivate;
    class SdlPlayWidget {

        public:
            SdlPlayWidget();
            ~SdlPlayWidget();

            void setSdlUISize(int,int);
            bool createPlayWindow(void *pWnd);
            void setWindowSize();
            void test();
            bool createSdlVedioPlayWidget();
            void InitWindow();
            void playVedioData(AVFrame *pFrameYUV);
            void playVedioDataArry(uint8_t *vedio_buffer_yuv,int num);
            void closeWindow();

        private:
            void clearEvn();

        private:
            SdlPlayWidgetPrivate *d;

    };

}

#endif
