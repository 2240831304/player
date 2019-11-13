
#include "realtimevedioplay.h"
#include <QDebug>
#include "sharedvariable.h"
#include <QDateTime>
#include "mp4package.h"
#include "packetlist.h"
#include <QMutex>
#include "openglplaywidget.h"


namespace eink {

    class RealTimeVedioPlayPrivate
    {
        public:
            RealTimeVedioPlayPrivate()
            {
                vedio_buffer = NULL;
                pFormatCtx = NULL;
                pCodecCtx = NULL;
                pFrameYUV = NULL;
                pFrame = NULL;
                isSaveVedioFile = false;
                mp4DataPackage = NULL;
                openglPlayer = NULL;
                sdlPlayer = NULL;
                vedio_buffer_yuv = NULL;
                pFrame_yuv = NULL;
                isRtmpPlugFlow = false;
            }

            AVFormatContext *pFormatCtx;
            AVCodecContext  *pCodecCtx;
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

            PacketList avpacketList;
            QMutex avpacketListMutex;
            OPenglPlayWidget *openglPlayer;

            RtmpPlugFlow *rtmpPlugFlowVedio;
            bool isRtmpPlugFlow;
    };



    //抓频操作
    RealTimeVedioPlay::RealTimeVedioPlay(QObject *parent)
        : QThread(parent), d(new RealTimeVedioPlayPrivate)
    {
        av_register_all();
        avdevice_register_all();
        packet_queue_init(&d->avpacketList);
    }

    RealTimeVedioPlay::~RealTimeVedioPlay()
    {
        freeMemory();
        if(d)
            delete d;
        d = NULL;
    }

    void RealTimeVedioPlay::saveFile()
    {
        if(!d->mp4DataPackage)
            d->mp4DataPackage = new Mp4Package;

        setFileName();
        d->mp4DataPackage->createVedioEncoder(d->pFormatCtx,d->pCodecCtx,d->time_base,d->frame_rate);
        d->mp4DataPackage->createAudioEncoder();
        d->mp4DataPackage->start();

        d->isSaveVedioFile = true;
    }

    void RealTimeVedioPlay::stop()
    {
    }

    void RealTimeVedioPlay::setFileName()
    {
        QDateTime time = QDateTime::currentDateTime();
        //QString fileName =  QString::number(time.toTime_t()) + ".mp4";
        QString fileName =  QString::number(time.date().year()) +
            QString::number(time.date().month()) + QString::number(time.date().day());
        fileName.append(QString::number(time.time().hour()));
        fileName.append(QString::number(time.time().minute()));
        fileName.append(QString::number(time.time().second()));
        fileName.append(".mp4");
        qDebug() << "RealTimeVedioPlay::setFileName() file name = " << fileName;
        d->mp4DataPackage->setFileName(fileName);
    }

    void RealTimeVedioPlay::InitEvn()
    {
        d->vedioIndex = -1;
        d->audioIndex = -1;
        d->subtitleIndex = -1;
        d->got_picture = -1;
        d->isWorking = true;
        freeMemory();
        d->isSaveVedioFile = false;
        d->frame_rate = 0;
        d->isRtmpPlugFlow = false;
    }

    void RealTimeVedioPlay::freeMemory()
    {
        if(d->vedio_buffer){
            delete d->vedio_buffer;
            d->vedio_buffer = NULL;
        }

        if(d->vedio_buffer_yuv) {
            delete d->vedio_buffer_yuv;
            d->vedio_buffer_yuv = NULL;
        }

        if(d->pCodecCtx) {
            avcodec_free_context(&d->pCodecCtx);
            d->pCodecCtx = NULL;
        }

        if(d->pFrameYUV) {
            av_frame_free(&d->pFrameYUV);
            d->pFrameYUV = NULL;
        }

        if(d->pFrame_yuv){
            av_frame_free(&d->pFrame_yuv);
            d->pFrame_yuv = NULL;
        }

        if(d->pFrame) {
            av_frame_free(&d->pFrame);
            d->pFrame = NULL;
        }

        if(d->mp4DataPackage) {
            delete d->mp4DataPackage;
            d->mp4DataPackage = NULL;
        }

        packet_queue_clear(&d->avpacketList);
    }

    void RealTimeVedioPlay::setRtmpPusher(RtmpPlugFlow *object)
    {
        d->rtmpPlugFlowVedio = object;
    }

    void RealTimeVedioPlay::startRtmpPushVedio()
    {
        bool flag = d->rtmpPlugFlowVedio->createVedioEncoder(d->pFormatCtx,
                        d->pCodecCtx,d->time_base,d->frame_rate);
        if(flag) {
            d->rtmpPlugFlowVedio->start();
            d->isRtmpPlugFlow = true;
        }
    }

    void RealTimeVedioPlay::run()
    {
        startWork();
    }

    void RealTimeVedioPlay::setSdlPlayWidget(eink::SdlPlayWidget *player)
    {
        d->sdlPlayer = player;
    }

    void RealTimeVedioPlay::setFormatCtx(AVFormatContext **ctx)
    {
        d->pFormatCtx = *ctx;
    }

    void RealTimeVedioPlay::startWork()
    {
        findStream();
    }

    void RealTimeVedioPlay::setWorkingState(bool flag)
    {
        d->isWorking = flag;
    }

    void RealTimeVedioPlay::findStream()
    {
        if(d->pFormatCtx == NULL) {
            qDebug() << "RealTimeVedioPlay::findStream pFormatCtx is null!!!";
            return;
        }

        for(int i = 0; i < d->pFormatCtx->nb_streams; i++) {
            if(d->pFormatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
                    d->audioIndex = i;
                    qDebug() << "RealTimeVedioPlay::findStream() find audio stream!!!!";
            } else if(d->pFormatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_SUBTITLE){
                    d->subtitleIndex = i;
            } else if(d->pFormatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
                d->vedioIndex = i;
                qDebug() << "RealTimeVedioPlay::findStream() find vedio stream!!!!";
            }
	    }

	    if (d->vedioIndex == -1) {
            qDebug() << "RealTimeVedioPlay::findStream() Didn't find a vedio stream!!!!";
		    avformat_close_input(&d->pFormatCtx);
		    return;
	    }

	    AVStream *stream = d->pFormatCtx->streams[d->vedioIndex];
        d->time_base = stream->time_base;
        d->frame_rate = stream->avg_frame_rate.num / stream->avg_frame_rate.den;
        qDebug() << "RealTimeVedioPlay::findStream() time_base num=" << d->time_base.num
                << d->time_base.den << "frame_rate=" << d->frame_rate;

        //查找解码器
        d->pCodecCtx = avcodec_alloc_context3(NULL);

        int ret = avcodec_parameters_to_context(d->pCodecCtx,
                        d->pFormatCtx->streams[d->vedioIndex]->codecpar);
        if( ret < 0) {
            qDebug() << "RealTimeVedioPlay::findStream() not decorder !!!!";
            avformat_close_input(&d->pFormatCtx);
            return;
        }

        //解码方式
        d->pCodec = avcodec_find_decoder(d->pCodecCtx->codec_id);
	    if (d->pCodec == NULL){
                qDebug() << "RealTimeVedioPlay::findStream() not find decorde method!!";
		    avformat_close_input(&d->pFormatCtx);
		    return;
	    }

        //打开解码器
        if (avcodec_open2(d->pCodecCtx, d->pCodec, NULL) < 0) {
            qDebug() << "RealTimeVedioPlay::findStream() open decoder faile!!";
                avcodec_free_context(&d->pCodecCtx);
                avformat_close_input(&d->pFormatCtx);
                return;
        }

        setConvertFormat();
        parseVedioData();
	}


    void RealTimeVedioPlay::setConvertFormat()
    {
        qDebug() << "RealTimeVedioPlay::setConvertFormat() avpacket pic width=,height=" << d->pCodecCtx->width
                        << d->pCodecCtx->height;
        desktopGrabPlayWidgetWidth_GL -= (desktopGrabPlayWidgetWidth_GL % 16);
        desktopGrabPlayWidgetHeight_GL -= (desktopGrabPlayWidgetHeight_GL % 16);

        //设置转化为yuv数据格式
        setTramslationToYuvSdlInfo();

         d->sdlPlayer->setSdlUISize(desktopGrabPlayWidgetWidth_GL,desktopGrabPlayWidgetHeight_GL);
         d->sdlPlayer->setWindowSize();

        //setTransToRgbInfo();
      }

      void RealTimeVedioPlay::decodeVedioPacket(AVPacket *dataPacket)
      {
          d->avpacketListMutex.lock();
          packet_queue_put(&d->avpacketList,dataPacket);
          d->avpacketListMutex.unlock();
      }

      void RealTimeVedioPlay::parseVedioData()
      {
          AVPacket pkt;
          d->pFrame = av_frame_alloc();

          while (d->isWorking) {
//              if(pkt.data != NULL)
//                  av_free_packet(&pkt);
            d->avpacketListMutex.lock();
            int ret = packet_queue_get(&d->avpacketList, &pkt);
            d->avpacketListMutex.unlock();
            if(ret < 0) {
                //qDebug() << "RealTimeVedioPlay::parseVedioData packet_queue_get < 0";
                continue;
            }

            ret = avcodec_decode_video2(d->pCodecCtx, d->pFrame, &d->got_picture,&pkt);
            if (ret <= 0) {
                qDebug() <<  "RealTimeVedioPlay::parseVedioData() decode error!!!!" ;
                continue ;
            }

             //在sdl中播放
             //tramslationToYuvSdlPlay();

            //推流
            if(d->isRtmpPlugFlow)
                d->rtmpPlugFlowVedio->fillVedioData(d->pFrame);

             //转化图片显示
             //parseFrameToImage();
             //msleep(25);
        }
    }

    void RealTimeVedioPlay::tramslationToYuvSdlPlay()
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

    void RealTimeVedioPlay::setTramslationToYuvSdlInfo()
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


    void RealTimeVedioPlay::parseFrameToImage()
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

    void RealTimeVedioPlay::setTransToRgbInfo()
    {
        d->pFrameYUV = av_frame_alloc();

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
    }

}
