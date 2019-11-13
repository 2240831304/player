
#include "cameragrab.h"
#include <QDebug>
#include "sharedvariable.h"
#include <QDateTime>
#include "mp4package.h"
#include "recordsound.h"
#include "realtimevedioplay.h"

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

    class CameraGrabPrivate
    {
        public:
            CameraGrabPrivate()
            {
                pFormatCtx = NULL;
                realTimeVedioPlay = NULL;
            }

            AVFormatContext *pFormatCtx;
	        int vedioIndex;
	        int audioIndex;
	        int subtitleIndex;
	        AVPacket *packet;
	        bool isWorking;
	        AVRational time_base;
	        int frame_rate;
	        eink::RealTimeVedioPlay *realTimeVedioPlay;
	        eink::SdlPlayWidget *sdlPlayWindow;
    };



    //抓频操作
    CameraGrab::CameraGrab(QObject *parent)
        : QObject(parent),d(new CameraGrabPrivate)
    {
        av_register_all();
        avdevice_register_all();
    }

    CameraGrab::~CameraGrab()
    {
        freeMemory();

        if(d)
            delete d;
        d = NULL;
    }

    void CameraGrab::start()
    {
        startWork();
    }

    void CameraGrab::saveFile()
    {
    }

    void CameraGrab::stop()
    {
    }

    void CameraGrab::InitEvn()
    {
        d->vedioIndex = -1;
        d->audioIndex = -1;
        d->subtitleIndex = -1;
        d->isWorking = true;
        freeMemory();
        d->frame_rate = 0;
    }

    void CameraGrab::setSdlPlayWidget(eink::SdlPlayWidget *player)
    {
        d->sdlPlayWindow = player;
    }

    void CameraGrab::freeMemory()
    {
        if(d->pFormatCtx){
            avformat_free_context(d->pFormatCtx);
            d->pFormatCtx = NULL;
        }

        if(d->realTimeVedioPlay){
            delete d->realTimeVedioPlay;
            d->realTimeVedioPlay = NULL;
        }
    }

    void CameraGrab::startWork()
    {
        InitEvn();

        d->pFormatCtx = avformat_alloc_context();
        if(d->realTimeVedioPlay == NULL){
            d->realTimeVedioPlay = new eink::RealTimeVedioPlay;
        }
        d->realTimeVedioPlay->setSdlPlayWidget(d->sdlPlayWindow);

        fillFormatInfo();
    }

    void CameraGrab::fillFormatInfo()
    {
        AVDictionary *options = NULL;
        av_dict_set(&options,"framerate","25",0);

        //Make the grabbed area follow the mouse
        av_dict_set(&options,"follow_mouse","centered",0);
        av_dict_set(&options, "preset", "veryslow", 0 );
        av_dict_set(&options, "tune", "zerolatency", 0);
        av_dict_set(&options, "profile", "main", 0);
        av_dict_set(&options,"crf","18",AV_OPT_SEARCH_CHILDREN);

        av_dict_set(&options, "bit_rate", "512000", 0 );

        //Video frame size. The default is to capture the full screen
        av_dict_set(&options,"video_size","704x576",0);
        //av_dict_set(&options,"show_region","1",0);

        AVInputFormat *ifmt = av_find_input_format("video4linux2");
        if(ifmt == NULL) {
            qDebug() << "CameraGrab::fillFormatInfo() no such device!! ";
            return;
        }

        int ret = avformat_open_input(&d->pFormatCtx,"/dev/video0",ifmt,NULL);
        if(ret != 0){
            printf("CameraGrab::fillFormatInfo() Couldn't open input stream.\n");
            return;
        }
        av_dict_free(&options);

        findStream();
    }

    void CameraGrab::findStream()
    {
        if(avformat_find_stream_info(d->pFormatCtx,NULL) < 0) {
            qDebug() << "CameraGrab:: Couldn't find stream information";
            avformat_close_input(&d->pFormatCtx);
            return;
        }

        for(int i = 0; i < d->pFormatCtx->nb_streams; i++) {
		    if(d->pFormatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
			    d->audioIndex = i;
			    qDebug() << "CameraGrab::findStream() find audio stream!!!!";
		    } else if(d->pFormatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_SUBTITLE){
			    d->subtitleIndex = i;
		    } else if(d->pFormatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
		        d->vedioIndex = i;
		        qDebug() << "CameraGrab::findStream() find vedio stream!!!!";
		    }
	    }

	    if (d->vedioIndex == -1) {
		    qDebug() << "CameraGrab::findStream() Didn't find a vedio stream!!!!";
		    avformat_close_input(&d->pFormatCtx);
		    return;
	    }

	    AVStream *stream = d->pFormatCtx->streams[d->vedioIndex];
        d->time_base = stream->time_base;
        d->frame_rate = stream->avg_frame_rate.num / stream->avg_frame_rate.den;
        qDebug() << "CameraGrab::findStream() time_base num=" << d->time_base.num
                << d->time_base.den << "frame_rate=" << d->frame_rate;

	    readFrameData();
	}


	void CameraGrab::setWorkingState(bool flag)
	{
	    d->isWorking = flag;
	    d->realTimeVedioPlay->setWorkingState(false);
        if(!flag && d->realTimeVedioPlay->isRunning()) {
            d->realTimeVedioPlay->quit();
            d->realTimeVedioPlay->wait();
        }
        avformat_close_input(&d->pFormatCtx);
	}

    void CameraGrab::readFrameData()
    {
        d->realTimeVedioPlay->InitEvn();
        d->realTimeVedioPlay->setFormatCtx(&d->pFormatCtx);
        d->realTimeVedioPlay->start();

        d->packet = (AVPacket *) malloc(sizeof(AVPacket)); //分配一个packet
        //下面两种办法分配空间都可以
        #if 1
            av_init_packet(d->packet);
        #else
            int y_size = d->pCodecCtx->width * d->pCodecCtx->height;
            av_new_packet(d->packet, y_size); //分配packet的数据
        #endif

        while (d->isWorking)
        {
            //av_read_frame读取的是一帧视频，并存入一个AVPacket的结构中
            if (av_read_frame(d->pFormatCtx, d->packet) < 0)
            {
                qDebug() << "CameraGrab::readFrameData(),parse file is finished,size=";
                continue; //这里认为视频读取完了
            }

            if (d->packet->stream_index == d->vedioIndex) {
                d->realTimeVedioPlay->decodeVedioPacket(d->packet);
            } else {
                av_free_packet(d->packet);
            }
        }
    }


}
