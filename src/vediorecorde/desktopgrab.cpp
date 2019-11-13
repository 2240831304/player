
#include "desktopgrab.h"
#include <QDebug>
#include "recordsound.h"
#include "realtimevedioplay.h"
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

    class DesktopGrabPrivate
    {
        public:
            DesktopGrabPrivate()
            {
                pFormatCtx = NULL;
                recordSoundObj = NULL;
                realTimeVedioPlay = NULL;
                rtmpPushVedio = NULL;
                isAllowPushVedio = false;
            }

            AVFormatContext *pFormatCtx;
            int vedioIndex;
            int audioIndex;
            int subtitleIndex;
            AVPacket *packet;
            bool isWorking;
            AVRational time_base;
            int frame_rate;
            RecordSound *recordSoundObj;
            eink::RealTimeVedioPlay *realTimeVedioPlay;
            RtmpPlugFlow *rtmpPushVedio;
            bool isAllowPushVedio;
            eink::SdlPlayWidget *sdlPlayWindow;
    };


    //抓频操作
    DesktopGrab::DesktopGrab(QObject *parent)
        : QObject(parent),d(new DesktopGrabPrivate)
    {
        av_register_all();
        avdevice_register_all();
    }

    DesktopGrab::~DesktopGrab()
    {
        freeMemory();
        if(d)
            delete d;
        d = NULL;
    }

    void DesktopGrab::InitEvn()
    {
        d->vedioIndex = -1;
        d->audioIndex = -1;
        d->subtitleIndex = -1;
        d->isWorking = true;
        d->frame_rate = 0;
        d->isAllowPushVedio = false;
        freeMemory();
    }

    void DesktopGrab::freeMemory()
    {
        if(d->pFormatCtx){
            avformat_free_context(d->pFormatCtx);
            d->pFormatCtx = NULL;
        }

        if(d->realTimeVedioPlay){
            delete d->realTimeVedioPlay;
            d->realTimeVedioPlay = NULL;
        }

        if(d->rtmpPushVedio){
            delete d->rtmpPushVedio;
            d->rtmpPushVedio  = NULL;
        }
    }


    void DesktopGrab::start()
    {
        startWork();
    }

    void DesktopGrab::saveFile()
    {
    }

    void DesktopGrab::setSdlPlayWidget(eink::SdlPlayWidget *player)
    {
        d->sdlPlayWindow = player;
    }


    void DesktopGrab::rtmpFlugVedio()
    {
        if(d->isAllowPushVedio) {
            d->realTimeVedioPlay->setRtmpPusher(d->rtmpPushVedio);
            d->realTimeVedioPlay->startRtmpPushVedio();
        }
    }

    void DesktopGrab::stop()
    {
    }

    void DesktopGrab::startWork()
    {
        InitEvn();

        if(!d->recordSoundObj)
            d->recordSoundObj = new RecordSound;

        if(d->realTimeVedioPlay == NULL){
            d->realTimeVedioPlay = new eink::RealTimeVedioPlay;
        }
        d->realTimeVedioPlay->setSdlPlayWidget(d->sdlPlayWindow);

        if(!d->rtmpPushVedio)
            d->rtmpPushVedio = new RtmpPlugFlow;
        d->isAllowPushVedio = d->rtmpPushVedio->openPushAddress();

        fillFormatInfo();
    }

    void DesktopGrab::setWorkingState(bool flag)
	{
	    d->isWorking = flag;

        d->recordSoundObj->setAudioPlayState(flag);
        if(!flag && d->recordSoundObj->isRunning()) {
            d->recordSoundObj->quit();
            d->recordSoundObj->wait(); //等待线程结束
        }

        d->realTimeVedioPlay->setWorkingState(false);
        if(!flag && d->realTimeVedioPlay->isRunning()) {
            d->realTimeVedioPlay->quit();
            d->realTimeVedioPlay->wait();
        }
	}

    void DesktopGrab::fillFormatInfo()
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

        AVInputFormat *InputFormat = av_find_input_format("x11grab");
        d->pFormatCtx = avformat_alloc_context();

        int ret = avformat_open_input(&d->pFormatCtx,"",InputFormat,&options);
        if(ret != 0 )
        {
            av_dict_free(&options);
            qDebug() << "DesktopGrab::fillFormatInfo() Couldn't open input stream,ret=" << ret;
            return;
        }

        av_dict_free(&options);
        findStream();
    }

    void DesktopGrab::findStream()
    {
        if(avformat_find_stream_info(d->pFormatCtx,NULL) < 0) {
            qDebug() << "DesktopGrab:: Couldn't find stream information";
            avformat_close_input(&d->pFormatCtx);
            return;
        }

        /* Output info */
        printf("-------------File Information-------------\n");
        av_dump_format(d->pFormatCtx, 0,0, 0);
        printf("------------------------------------------\n");

        for(int i = 0; i < d->pFormatCtx->nb_streams; i++) {
		    if(d->pFormatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
			    d->audioIndex = i;
			    qDebug() << "DesktopGrab::findStream() find audio stream!!!!";
		    } else if(d->pFormatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_SUBTITLE){
			    d->subtitleIndex = i;
		    } else if(d->pFormatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
		        d->vedioIndex = i;
		        qDebug() << "DesktopGrab::findStream() find vedio stream!!!!";
		    }
	    }

	    if (d->vedioIndex == -1) {
		    qDebug() << "DesktopGrab::findStream() Didn't find a vedio stream!!!!";
		    avformat_close_input(&d->pFormatCtx);
		    return;
	    }

	    AVStream *stream = d->pFormatCtx->streams[d->vedioIndex];
        d->time_base = stream->time_base;
        d->frame_rate = stream->avg_frame_rate.num / stream->avg_frame_rate.den;
        qDebug() << "DesktopGrab::findStream() time_base num=" << d->time_base.num
                << d->time_base.den << "frame_rate=" << d->frame_rate;

	    readFrameData();
	}

    void DesktopGrab::readFrameData()
    {
        d->packet = (AVPacket *) malloc(sizeof(AVPacket)); //分配一个packet
        //下面两种办法分配空间都可以
        #if 1
            av_init_packet(d->packet);
        #else
            int y_size = d->pCodecCtx->width * d->pCodecCtx->height;
            av_new_packet(d->packet, y_size); //分配packet的数据
        #endif

        d->realTimeVedioPlay->InitEvn();
        d->realTimeVedioPlay->setFormatCtx(&d->pFormatCtx);
        d->realTimeVedioPlay->start();

        //获取声音数据
        //d->recordSoundObj->start();

        while (d->isWorking)
        {
            //av_read_frame读取的是一帧视频，并存入一个AVPacket的结构中
            if (av_read_frame(d->pFormatCtx, d->packet) < 0)
            {
                qDebug() << "DesktopGrab::readFrameData(),parse file is finished,size=";
                break; //这里认为视频读取完了
            }

            if (d->packet->stream_index == d->vedioIndex) {
                d->realTimeVedioPlay->decodeVedioPacket(d->packet);
            } else if(d->packet->stream_index == d->audioIndex) {
                av_free_packet(d->packet);
            } else {
                av_free_packet(d->packet);
            }
        }

    }

}
