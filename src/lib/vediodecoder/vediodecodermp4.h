#ifndef VEDIODECODERMP4_H
#define VEDIODECODERMP4_H

extern "C"{
    #include <libavcodec/avcodec.h>
    #include <libavformat/avformat.h>
    #include <libavutil/pixfmt.h>
    #include <libswscale/swscale.h>
	#include <libavutil/fifo.h>
	#include <libavutil/mem.h>
	#include <SDL2/SDL.h>
}

#include <QObject>
#include <QImage>
#include "vediodecoder.h"

namespace eink {

	class VedioDecoderMp4Private;

	class VedioDecoderMp4 : public QObject,public VedioDecoder {
		Q_OBJECT
		public :
			VedioDecoderMp4(QObject *parent = 0);
			~VedioDecoderMp4();

			void startParse();

			virtual void play();
            virtual void stop();
            virtual void suspendPlay();
            virtual void keepOnPlay();
            virtual void adjustPlayTiemPos(int ) ;
            virtual const char *getVedioPlayerType();
            virtual void setFilePath(const std::string &);
            virtual void setParseState(bool);


		private :
            void ResetEvn();
			void openVedioFile();
			void findVedionStreamType();
			void createDecompiler();
			void readFrameData();
			void dataDecompression();
			void translateCode();
			void decoderVedio();

			void parseFrameToImage(int,int);
                        void determinePicType();

		signals:
			void SendImage(QImage );
			void sendIntSignal();

		private :
			VedioDecoderMp4Private *d;

	};
}

#endif
