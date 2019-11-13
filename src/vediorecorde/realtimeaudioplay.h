#ifndef RECORDSOUND_H
#define RECORDSOUND_H

#include <iostream>
#include <QThread>

extern "C"{
    #include <libavcodec/avcodec.h>
    #include <libavformat/avformat.h>
    #include <libavutil/pixfmt.h>
    #include <libswscale/swscale.h>
    #include <libavutil/fifo.h>
    #include <libavutil/mem.h>
    #include <libswresample/swresample.h>
    #include <SDL2/SDL.h>
    #include <libavutil/opt.h>
    #include <libavutil/channel_layout.h>
    #include <libavutil/samplefmt.h>
}

typedef unsigned char Uint8;

namespace eink {

    class RecordSoundPrivate;
    class RecordSound : public QThread {
        public:
            RecordSound();
            ~RecordSound();

            void setFilePath(const std::string &);
            void setMicrophoneTool(const std::string &);
            void setMicrophoneDevice(const std::string &);
            void startParse();
            void setAudioPlayState(bool );
            bool getAudioPlayState();


        private:
            void InitEvn();
            static void audio_callback(void *userdata, Uint8 *stream, int len);
            void freeMemory();
            void findAudioStreamType();
            void openDecompiler();
            void openAudioDevice();
            void read_device_Data();
            int  audio_decode_apcket(AVCodecContext *pcodec_ctx);
            void set_swr_convert_Info();

        protected:
            void run();

        private:
            RecordSoundPrivate *d;
            static RecordSound *StaticObject;
    };

}
#endif
