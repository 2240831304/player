
#include "rtmpplugflow.h"
#include "systempath.h"
#include "sharedvariable.h"
#include <QDebug>

namespace eink{

    class RtmpPlugFlowPrivate {
        public:
            RtmpPlugFlowPrivate()
            {
                isAllowWriteData = false;
            }

            std::string filePath;

            AVFormatContext *pFormatCtx;
            AVCodecContext	*videoCodecCtx;
            AVCodecContext	*audioCodecCtx;
            AVFrame	*pFrame;
            AVStream *VideoStream;
            AVStream *AudioStream;
            bool isAllowWriteData;
            uint8_t *vedio_buffer;
            AVFrame *pFrameYUV;
            struct SwsContext *img_convert_ctx;
            int pFrameNum;
            int frame_rate;
    };



    RtmpPlugFlow::RtmpPlugFlow()
        : d(new RtmpPlugFlowPrivate)
    {
        av_register_all();
        avdevice_register_all();
    }

    RtmpPlugFlow::~RtmpPlugFlow()
    {
    }

    void RtmpPlugFlow::setFileName(const QString &fileName)
    {
        QString path = SystemPath::getFilesPath() + "/vedio/" + fileName;
        d->filePath = path.toStdString();
        qDebug() << "RtmpPlugFlow::setFileName file name==" << path;
    }

    void RtmpPlugFlow::InitEvn()
    {
        d->isAllowWriteData = false;
        d->pFrameNum = 0;
        d->frame_rate = 0;
    }

    void RtmpPlugFlow::freeMemory()
    {
        InitEvn();

        if (d->pFormatCtx) {
            avformat_close_input(&d->pFormatCtx); // 关闭封装上下文
        }
        // 关闭编码器和清理上下文的所有空间
        if (d->videoCodecCtx) {
            avcodec_close(d->videoCodecCtx);
            avcodec_free_context(&d->videoCodecCtx);
        }

        if (d->audioCodecCtx) {
            avcodec_close(d->audioCodecCtx);
            avcodec_free_context(&d->audioCodecCtx);
        }
    }

    void RtmpPlugFlow::start()
    {
        writeHeader();
    }


    void RtmpPlugFlow::stop()
    {
        writeTrailer();
        freeMemory();
    }

    void RtmpPlugFlow::writeHeader()
    {
        if(avio_open(&d->pFormatCtx->pb,"rtmp://localhost:1935/live/film.flv", AVIO_FLAG_READ_WRITE) < 0)
        {
            qDebug() << "RtmpPlugFlow::prepareSaveInfo() Failed to open output file! 输出文件打开失败";
            return;
        }

        // 写文件头
        int ret = avformat_write_header(d->pFormatCtx, NULL);
        if (ret < 0) {
            qDebug() <<  "RtmpPlugFlow::prepareSaveInfo() avformat_write_header failed";
            return;
        }

        d->isAllowWriteData = true;
    }

    void RtmpPlugFlow::writeTrailer()
    {
        if(!d->isAllowWriteData)
            return;
        // 写入文件尾
        int ret = av_write_trailer(d->pFormatCtx);
        if (ret != 0) {
            qDebug() <<  "RtmpPlugFlow::writeTrailer() av_write_trailer failed";
            return;
        }

        ret = avio_closep(&d->pFormatCtx->pb); // 关闭AVIO流
        if (ret != 0) {
            qDebug() << "RtmpPlugFlow::writeTrailer() avio_close failed";
        }
    }

    bool RtmpPlugFlow::openPushAddress()
    {
        d->pFormatCtx = avformat_alloc_context();

        int ret = avformat_alloc_output_context2(&d->pFormatCtx,NULL,
                                "flv","rtmp://localhost:1935/live/film");
        if (ret < 0) {
            qDebug() << "RtmpPlugFlow::createVedioEncoder avformat_alloc_output_context2 faile!!";
            return false;
        }
        return true;
    }

    bool RtmpPlugFlow::createVedioEncoder(const AVFormatContext *formatCtx,
            const AVCodecContext *coderCtx,const AVRational &rational,int frameRate)
    {
        InitEvn();

        d->frame_rate = frameRate;

        // h264视频编码器  AVCodecID::AV_CODEC_ID_H264
        const AVCodec *vcodec = avcodec_find_encoder(AVCodecID::AV_CODEC_ID_H264);
        if (!vcodec) {
            qDebug() << "RtmpPlugFlow::createVedioEncoder() create vcodec faile!!";
            return false;
        }
        // 创建编码器上下文
        d->videoCodecCtx = avcodec_alloc_context3(vcodec);
        if (!d->videoCodecCtx) {
            qDebug() << "RtmpPlugFlow::createVedioEncoder() create videoCodecCtx faile!!";
            return false;
        }

        qDebug() << "RtmpPlugFlow::createVedioEncoder origin data bit_rate=" << coderCtx->bit_rate;

        // 比特率、宽度、高度
        d->videoCodecCtx->bit_rate = 5120000;
        d->videoCodecCtx->width = coderCtx->width;// 视频宽度
        d->videoCodecCtx->height = coderCtx->height;// 视频高度
        d->videoCodecCtx->codec_type = AVMEDIA_TYPE_VIDEO;
        // 时间基数、帧率
        //AVRational rFrame = { 1, 25 };   //每秒25帧
        d->videoCodecCtx->time_base.num = 1;
        d->videoCodecCtx->time_base.den = frameRate;
        //固定允许的码率误差，数值越大，视频越小
        //d->videoCodecCtx->bit_rate_tolerance = 4000000;
        // 关键帧间隔
        //d->videoCodecCtx->gop_size = 10;
        // 不使用b帧
        d->videoCodecCtx->max_b_frames = 0;
        // 帧、编码格式
        d->videoCodecCtx->pix_fmt = AVPixelFormat::AV_PIX_FMT_YUV420P;
        d->videoCodecCtx->codec_id = AVCodecID::AV_CODEC_ID_H264;

        // 全局头
        //d->videoCodecCtx->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
        //编码速度&编码质量&视觉优化参数
        av_opt_set(d->videoCodecCtx->priv_data, "preset", "medium", 0);
        av_opt_set(d->videoCodecCtx->priv_data, "tune", "zerolatency", 0);
        av_opt_set(d->videoCodecCtx->priv_data, "profile", "main", 0);
        av_opt_set(d->videoCodecCtx->priv_data, "crf","18",AV_OPT_SEARCH_CHILDREN);

        int ret = avcodec_open2(d->videoCodecCtx, vcodec, NULL);
        if (ret  < 0) {
            qDebug() << "RtmpPlugFlow::createVedioEncoder() avcodec_open2 failed!";
            return false;
        }

        // 为封装器创建视频流
        d->VideoStream = avformat_new_stream(d->pFormatCtx, vcodec);
        if (!d->VideoStream) {
            qDebug() << "RtmpPlugFlow::createVedioEncoder() avformat_new_stream video stream failed!";
            return false;
        }

        d->VideoStream->index = AVMEDIA_TYPE_VIDEO;
        d->VideoStream->codecpar->codec_type = AVMEDIA_TYPE_VIDEO;
        d->VideoStream->time_base = rational;
        d->VideoStream->avg_frame_rate.num = frameRate;
        d->VideoStream->avg_frame_rate.den = 1;
        // 配置视频流的编码参数
        avcodec_parameters_from_context(d->VideoStream->codecpar, d->videoCodecCtx);

        //设置图像转换信息
        setScaleInfo(formatCtx,coderCtx);

        return true;
    }

    void RtmpPlugFlow::fillVedioData(const AVFrame *vframe)
    {
        if(!d->isAllowWriteData)
            return;

        d->pFrameYUV->pts = 0.04 * d->pFrameNum / av_q2d(d->VideoStream->time_base);
        //qDebug() << "S55555555555555555555555555555555=d->pFrameYUV->pts"
        //        << d->pFrameYUV->pts;

        sws_scale(d->img_convert_ctx,
                (uint8_t const * const *) vframe->data,
                vframe->linesize, 0, d->videoCodecCtx->height, d->pFrameYUV->data,
                d->pFrameYUV->linesize);

        // 视频编码报文
        AVPacket *vpkt = av_packet_alloc();
        int get_packet = -1;
        int ret = avcodec_encode_video2(d->videoCodecCtx,vpkt,d->pFrameYUV,&get_packet);
        if( (ret < 0) || (get_packet != 1) ) {
            qDebug() << "RtmpPlugFlow::fillVedioData encode faile!!!,ret = " << get_packet;
            return;
        }

        // 转换pts
        //av_packet_rescale_ts(vpkt, d->videoCodecCtx->time_base, d->VideoStream->time_base);

        vpkt->stream_index = d->VideoStream->index;
        vpkt->pts = d->pFrameYUV->pts;

        // 向封装器中写入压缩报文，该函数会自动释放pkt空间，不需要调用者手动释放
        ret = av_interleaved_write_frame(d->pFormatCtx, vpkt);
        if (ret < 0) {
            qDebug() << "RtmpPlugFlow::fillVedioAvpacket av_interleaved_write_frame failed";
        }
        av_free_packet(vpkt);

        d->pFrameNum += 1;

    }

    void RtmpPlugFlow::setScaleInfo(const AVFormatContext *,const AVCodecContext *codecContext)
    {
        d->pFrameYUV = av_frame_alloc();
        d->pFrameYUV->width = d->videoCodecCtx->width;
        d->pFrameYUV->height = d->videoCodecCtx->height;
        d->pFrameYUV->format = AV_PIX_FMT_YUV420P;

        //设置格式转换后的 宽和高
        d->img_convert_ctx = sws_getContext(codecContext->width, codecContext->height,
                codecContext->pix_fmt, d->videoCodecCtx->width, d->videoCodecCtx->height,
                AV_PIX_FMT_YUV420P, SWS_BICUBIC, NULL, NULL, NULL);

        //一张图片需要的内存空间
        int numBytes = avpicture_get_size(AV_PIX_FMT_YUV420P, d->videoCodecCtx->width,
                                        d->videoCodecCtx->height);

        d->vedio_buffer = (uchar *) malloc(numBytes * sizeof(uchar));

        avpicture_fill((AVPicture *) d->pFrameYUV, d->vedio_buffer, AV_PIX_FMT_YUV420P,
                d->videoCodecCtx->width, d->videoCodecCtx->height);
    }


    void RtmpPlugFlow::createAudioEncoder()
    {
        qDebug() << "RtmpPlugFlow::createAudioEncoder() @@@@@@@@@@@@@@@";
        // 创建音频编码器，指定类型为AAC
        const AVCodec *acodec = avcodec_find_encoder(AVCodecID::AV_CODEC_ID_AAC);
        if(!acodec) {
            qDebug() << "RtmpPlugFlow::createAudioEncoder create audio encoder faile!!";
            return;
        }

        // 根据编码器创建编码器上下文
        d->audioCodecCtx = avcodec_alloc_context3(acodec);
        if(!d->audioCodecCtx) {
            qDebug() << "RtmpPlugFlow::createAudioEncoder avcodec_alloc_context3 faile!!";
            return;
        }

        // 比特率、采样率、采样类型、音频通道、文件格式
        d->audioCodecCtx->bit_rate = 64000;
        d->audioCodecCtx->sample_rate = 44100;
        d->audioCodecCtx->sample_fmt = AVSampleFormat::AV_SAMPLE_FMT_FLTP;
        d->audioCodecCtx->channels = 2;
        // 根据音频通道数自动选择输出类型（默认为立体声）
        d->audioCodecCtx->channel_layout = av_get_default_channel_layout(2);
        d->audioCodecCtx->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;

        // 打开编码器
        int ret = avcodec_open2(d->audioCodecCtx, acodec, NULL);
        if(ret < 0) {
            avcodec_free_context(&d->audioCodecCtx);
            qDebug() << "RtmpPlugFlow::createAudioEncoder open audio encoder faile!!";
            return;
        }

        // 添加音频流
        d->AudioStream = avformat_new_stream(d->pFormatCtx, NULL);
        if(!d->AudioStream) {
            qDebug() << "RtmpPlugFlow::createAudioEncoder create audio strean faile!!";
            return;
        }

        d->AudioStream->index = AVMEDIA_TYPE_AUDIO;
        d->AudioStream->codecpar->codec_type = AVMEDIA_TYPE_AUDIO;
        // 配置音频流的编码器参数
        avcodec_parameters_from_context(d->AudioStream->codecpar, d->audioCodecCtx);

    }

    void RtmpPlugFlow::setResampleInfo()
    {
    }

    void RtmpPlugFlow::fillAudioData(const AVFrame *)
    {
    }

}
