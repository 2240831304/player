
#include "lovelive.h"
#include <QDebug>
#include "sharedvariable.h"
#include <QDateTime>
#include "mp4package.h"
#include "realtimevedioplay.h"
#include "openglplaywidget.h"

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

    class LoveLivePrivate
    {
        public:
            LoveLivePrivate()
            {
                vedio_buffer = NULL;
                pFormatCtx = NULL;
                pCodecCtx = NULL;
                pFrameYUV = NULL;
                pFrame = NULL;
                isSaveVedioFile = false;
                mp4DataPackage = NULL;
                realTimeVedioPlay = NULL;
            }

            AVFormatContext *pFormatCtx;
            AVCodecContext	*pCodecCtx;
            AVCodec *pCodec;
            int vedioIndex;
            int audioIndex;
            int subtitleIndex;
            AVFrame	*pFrame;
            AVFrame *pFrameYUV;
            struct SwsContext *img_convert_ctx;
            uint8_t *vedio_buffer;
            AVPacket *packet;
            int got_picture;
            bool isWorking;
            AVRational time_base;
            bool isSaveVedioFile;
            Mp4Package *mp4DataPackage;
            int frame_rate;
            eink::SdlPlayWidget *sdlPlayer;

            uint8_t *vedio_buffer_yuv;
            AVFrame *pFrame_yuv;
            struct SwsContext *sws_sdl_yuv;
            eink::RealTimeVedioPlay *realTimeVedioPlay;
            bool isSdlWindowCreate;
    };



    //直播
    LoveLive::LoveLive(QObject *parent)
        : QObject(parent),d(new LoveLivePrivate)
    {
        av_register_all();
        avdevice_register_all();
    }

    LoveLive::~LoveLive()
    {
        if(d->mp4DataPackage)
            delete d->mp4DataPackage;
        d->mp4DataPackage = NULL;

        freeMemory();
        if(d)
            delete d;
        d = NULL;
    }

    void LoveLive::setSdlPlayWidget(eink::SdlPlayWidget *player)
    {
        d->sdlPlayer = player;
    }

    void LoveLive::start()
    {
        startWork();
    }

    void LoveLive::saveFile()
    {
        if(!d->mp4DataPackage)
            d->mp4DataPackage = new Mp4Package;

        setFileName();
        d->mp4DataPackage->createVedioEncoder(d->pFormatCtx,d->pCodecCtx,d->time_base,d->frame_rate);
        d->mp4DataPackage->createAudioEncoder();
        d->mp4DataPackage->start();

        d->isSaveVedioFile = true;
    }

    void LoveLive::stop()
    {
    }

    void LoveLive::setFileName()
    {
        QDateTime time = QDateTime::currentDateTime();
        //QString fileName =  QString::number(time.toTime_t()) + ".mp4";
        QString fileName =  QString::number(time.date().year()) +
            QString::number(time.date().month()) + QString::number(time.date().day());
        fileName.append(QString::number(time.time().hour()));
        fileName.append(QString::number(time.time().minute()));
        fileName.append(QString::number(time.time().second()));
        fileName.append(".mp4");
        qDebug() << "LoveLive::setFileName() file name = " << fileName;
        d->mp4DataPackage->setFileName(fileName);
    }

    void LoveLive::InitEvn()
    {
        freeMemory();
        d->vedioIndex = -1;
        d->audioIndex = -1;
        d->subtitleIndex = -1;
        d->got_picture = -1;
        d->isWorking = true;
        d->isSaveVedioFile = false;
        d->frame_rate = 0;
    }

    void LoveLive::freeMemory()
    {
        if(d->vedio_buffer){
            delete d->vedio_buffer;
            d->vedio_buffer = NULL;
        }

        if(d->pFormatCtx){
            avformat_free_context(d->pFormatCtx);
            d->pFormatCtx = NULL;
        }

        if(d->pCodecCtx) {
            avcodec_free_context(&d->pCodecCtx);
            d->pCodecCtx = NULL;
        }

        if(d->pFrameYUV) {
            av_frame_free(&d->pFrameYUV);
            d->pFrameYUV = NULL;
        }

        if(d->pFrame) {
            av_frame_free(&d->pFrame);
            d->pFrame = NULL;
        }

        if(d->realTimeVedioPlay){
            delete d->realTimeVedioPlay;
            d->realTimeVedioPlay = NULL;
        }

    }

    void LoveLive::startWork()
    {
        InitEvn();

        d->pFormatCtx = avformat_alloc_context();
        if(d->realTimeVedioPlay == NULL){
            d->realTimeVedioPlay = new eink::RealTimeVedioPlay;
        }

        fillFormatInfo();
    }

    void LoveLive::setWorkingState(bool flag)
    {
        d->isWorking = flag;
        d->realTimeVedioPlay->setWorkingState(false);
        if(!flag) {
            if(d->realTimeVedioPlay->isRunning()) {
                d->realTimeVedioPlay->quit();
                d->realTimeVedioPlay->wait();
            }
        }
    }

    void LoveLive::fillFormatInfo()
    {
        AVDictionary *options = NULL;
        //av_dict_set(&options,"framerate","25",0);

        //Make the grabbed area follow the mouse
        //av_dict_set(&options,"follow_mouse","centered",0);
        //av_dict_set(&options, "preset", "veryslow", 0 );
        //av_dict_set(&options, "tune", "zerolatency", 0);
        //av_dict_set(&options, "profile", "main", 0);
        //av_dict_set(&options,"crf","18",AV_OPT_SEARCH_CHILDREN);

        //av_dict_set(&options, "bit_rate", "512000", 0 );

        //Video frame size. The default is to capture the full screen
        //av_dict_set(&options,"video_size","704x576",0);
        //av_dict_set(&options,"show_region","1",0);

        int ret = avformat_open_input(&d->pFormatCtx,"rtmp://58.200.131.2:1935/livetv/hunantv",0,0);
        if(ret != 0 )
        {
            av_dict_free(&options);
            qDebug() << "LoveLive::fillFormatInfo() Couldn't open input stream,ret=" << ret;
            return;
        }

        findStream();
        readFrameData();
        //createCodec();
        //readFrameDataLocalParse();
    }

    void LoveLive::findStream()
    {
        if(avformat_find_stream_info(d->pFormatCtx,NULL) < 0) {
            qDebug() << "LoveLive:: Couldn't find stream information";
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
			    qDebug() << "LoveLive::findStream() find audio stream!!!!";
		    } else if(d->pFormatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_SUBTITLE){
			    d->subtitleIndex = i;
		    } else if(d->pFormatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
		        d->vedioIndex = i;
		        qDebug() << "LoveLive::findStream() find vedio stream!!!!";
		    }
        }

	    if (d->vedioIndex == -1) {
		    qDebug() << "LoveLive::findStream() Didn't find a vedio stream!!!!";
		    avformat_close_input(&d->pFormatCtx);
		    return;
	    }

	    AVStream *stream = d->pFormatCtx->streams[d->vedioIndex];
        d->time_base = stream->time_base;
        d->frame_rate = stream->avg_frame_rate.num / stream->avg_frame_rate.den;
        qDebug() << "LoveLive::findStream() time_base num=" << d->time_base.num
                << d->time_base.den << "frame_rate=" << d->frame_rate;
    }

    void LoveLive::createCodec()
    {
        //查找解码器
        d->pCodecCtx = avcodec_alloc_context3(NULL);

        int ret = avcodec_parameters_to_context(d->pCodecCtx,
                        d->pFormatCtx->streams[d->vedioIndex]->codecpar);
        if( ret < 0) {
            qDebug() << "LoveLive::findStream() not decorder !!!!";
            avformat_close_input(&d->pFormatCtx);
            return;
        }

        //解码方式
        d->pCodec = avcodec_find_decoder(d->pCodecCtx->codec_id);
	    if (d->pCodec == NULL){
	        qDebug() << "LoveLive::findStream() not find decorde method!!";
		    avformat_close_input(&d->pFormatCtx);
		    return;
	    }

	    //打开解码器
	    if (avcodec_open2(d->pCodecCtx, d->pCodec, NULL) < 0) {
	        qDebug() << "LoveLive::findStream() open decoder faile!!";
		    avcodec_free_context(&d->pCodecCtx);
		    avformat_close_input(&d->pFormatCtx);
		    return;
	    }
    }

    void LoveLive::readFrameData()
    {
        d->realTimeVedioPlay->InitEvn();
        d->realTimeVedioPlay->setSdlPlayWidget(d->sdlPlayer);
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
                qDebug() << "LoveLive::readFrameData(),parse file is finished,size=";
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


    void LoveLive::readFrameDataLocalParse()
    {
        d->pFrame = av_frame_alloc();
        d->pFrameYUV = av_frame_alloc();

        qDebug() << "LoveLive::readFrameData() avpacket pic width=,height=" << d->pCodecCtx->width
                        << d->pCodecCtx->height;
        desktopGrabPlayWidgetWidth_GL -= (desktopGrabPlayWidgetWidth_GL % 16);
        desktopGrabPlayWidgetHeight_GL -= (desktopGrabPlayWidgetHeight_GL % 16);

        //设置格式转换后的 宽和高
        d->img_convert_ctx = sws_getContext(d->pCodecCtx->width, d->pCodecCtx->height,
                d->pCodecCtx->pix_fmt, desktopGrabPlayWidgetWidth_GL, desktopGrabPlayWidgetHeight_GL,
                AV_PIX_FMT_RGB32, SWS_BICUBIC, NULL, NULL, NULL);

        //一张图片需要的内存空间
        int numBytes = avpicture_get_size(AV_PIX_FMT_BGR32, desktopGrabPlayWidgetWidth_GL,
                                        desktopGrabPlayWidgetHeight_GL);

        d->vedio_buffer = (uchar *) malloc(numBytes * sizeof(uchar));

        avpicture_fill((AVPicture *) d->pFrameYUV, d->vedio_buffer, AV_PIX_FMT_BGR32,
                desktopGrabPlayWidgetWidth_GL, desktopGrabPlayWidgetHeight_GL);

        //转化为yuv数据
        setTramslationToSdlInfo();

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
                qDebug() << "LoveLive::readFrameData(),parse file is finished,size=";
                break; //这里认为视频读取完了
            }

            if (d->packet->stream_index == d->vedioIndex) {
                int ret = avcodec_decode_video2(d->pCodecCtx, d->pFrame, &d->got_picture,d->packet);
                if (ret <= 0) {
                    printf("decode error!!!!");
                    continue ;
                }
                //qDebug() << "@@@@@@@@@@@@@@@@@@@@@@@@@@@2==" << d->packet->size << ret;

                //sdl播放
                if(d->isSdlWindowCreate)
                    tramslationToSdlDataPlay();

                //转化为图片播放
                parseFrameToImage();
                //往视频封装容器写入数据
                if(d->mp4DataPackage && d->isWorking)
                    d->mp4DataPackage->fillVedioData(d->pFrame);
            }
            av_free_packet(d->packet);
        }

        if(d->mp4DataPackage)
            d->mp4DataPackage->stop();

        sws_freeContext(d->img_convert_ctx);
    }



    void LoveLive::parseFrameToImage()
    {
        //将得到的YUV420数据转换成RGB格式
        if (d->got_picture) {
            sws_scale(d->img_convert_ctx,
                    (uint8_t const * const *) d->pFrame->data,
                    d->pFrame->linesize, 0, d->pCodecCtx->height, d->pFrameYUV->data,
                    d->pFrameYUV->linesize);

            //1.把解析得到的视频图片发送出去，或2.存放在全局变量中
            QImage Img((uchar *)d->vedio_buffer, desktopGrabPlayWidgetWidth_GL,
                desktopGrabPlayWidgetHeight_GL, QImage::Format_RGB32);

            emit sendImageSig(Img);

        } else {
            //qDebug() << "d->packet->pts=====" << d->packet->pts * av_q2d(d->time_base) * 1000
            //           << "d->packet->dts" << d->packet->dts * av_q2d(d->time_base) * 1000;
        }
    }

    void LoveLive::tramslationToSdlDataPlay()
    {
        //转换的YUV420数据格式
        if (d->got_picture) {
            sws_scale(d->sws_sdl_yuv,
                    (uint8_t const * const *) d->pFrame->data,
                    d->pFrame->linesize, 0, d->pCodecCtx->height, d->pFrame_yuv->data,
                    d->pFrame_yuv->linesize);

            d->sdlPlayer->playVedioData(d->pFrame_yuv);

        } else {
            //qDebug() << "d->packet->pts=====" << d->packet->pts * av_q2d(d->time_base) * 1000
            //           << "d->packet->dts" << d->packet->dts * av_q2d(d->time_base) * 1000;
        }
    }

    void LoveLive::setTramslationToSdlInfo()
    {
        d->pFrame_yuv = av_frame_alloc();
        //设置格式转换后的 宽和高
        d->sws_sdl_yuv = sws_getContext(d->pCodecCtx->width, d->pCodecCtx->height,
                d->pCodecCtx->pix_fmt, desktopGrabPlayWidgetWidth_GL, desktopGrabPlayWidgetHeight_GL,
                AV_PIX_FMT_YUV420P, SWS_BICUBIC, NULL, NULL, NULL);

        //一张图片需要的内存空间
        int numBytes = avpicture_get_size(AV_PIX_FMT_YUV420P, desktopGrabPlayWidgetWidth_GL,
                                        desktopGrabPlayWidgetHeight_GL);

        d->vedio_buffer_yuv = (uchar *) malloc(numBytes * sizeof(uchar));

        avpicture_fill((AVPicture *) d->pFrame_yuv, d->vedio_buffer_yuv, AV_PIX_FMT_YUV420P,
                desktopGrabPlayWidgetWidth_GL, desktopGrabPlayWidgetHeight_GL);

    }


}
