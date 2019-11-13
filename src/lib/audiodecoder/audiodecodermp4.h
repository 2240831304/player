
#ifndef AudioDecoderMp4MP4_H
#define AudioDecoderMp4MP4_H

#include <QObject>

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

#include "audiodecoder.h"

namespace eink {
	class AudioDecoderMp4Private;

	class AudioDecoderMp4 : public QObject,public AudioDecoder {
		Q_OBJECT
		public :
			AudioDecoderMp4(QObject *parent = 0);
			~AudioDecoderMp4();

		public :
		    void startParse();
			void  packetEnqueue(AVPacket *packet);

            bool getAudioDecoderMp4State();
            int getAudioPacketSize();
            
            virtual void play();
            virtual void stop();
            virtual void suspendPlay();
            virtual void keepOnPlay();
            virtual void adjustPlayTiemPos(int ) ;
            virtual const char *getVedioPlayerType();
            virtual void setFilePath(const std::string &);
            virtual void setParseState(bool);

		private :
		    void resetEvn();
		    void openFile();
			void createDecompiler();
			void findVedionStreamType();
			void readData();

            static void audio_callback(void *userdata, Uint8 *stream, int len);
            void freeMemory();

            void createAudioFile();

		public :
			int audio_decode_frame(AVCodecContext *pcodec_ctx);

		private :
		    AudioDecoderMp4Private *d;
			static AudioDecoderMp4 *CurMy;

	};
}

#endif
