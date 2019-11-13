#ifndef AUDIOFILEDECODERFLAC_H
#define AUDIOFILEDECODERFLAC_H

#include <QObject>
#include "audiofiledecoder.h"

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



namespace eink {
	    class AudioDecoderPrivate;

        class AudiofileDecoderFlac : public QObject,public AudioFileDecoder {
		Q_OBJECT
		public :
            AudiofileDecoderFlac(QObject *parent = 0);
            ~AudiofileDecoderFlac();

		public :
			void  packetEnqueue(AVPacket *packet);

			void setAudioDecoderState(bool);
            bool getAudioDecoderState();
            int getAudioPacketSize();

            virtual void play();
		    virtual void stop();
		    virtual void suspendPlay();
		    virtual void keepOnPlay();
		    virtual const char *getAudioPlayerType();
		    virtual void setFilePath(const std::string &);
		    virtual bool getPlayFinishedState();
		    virtual void adjustPlayTimePos(int );

		    void quitPlay();

		private :
		    void InitEvn();
		    void startParse();
		    void openFile();
			void createDecompiler();
			void findVedionStreamType();
			void openAudioDevice();
			void readData();

            static void audio_callback(void *userdata, Uint8 *stream, int len);
            void freeMemory();

            void createAudioFile();

			int audio_decode_frame(AVCodecContext *pcodec_ctx);
			void avSeekPacket(int);

		private :
		    AudioDecoderPrivate *d;
            static AudiofileDecoderFlac *CurMy;

	};
}

#endif
