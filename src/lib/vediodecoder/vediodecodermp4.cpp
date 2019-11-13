
#include "vediodecodermp4.h"
#include <iostream>
#include <stdio.h>
#include <QThread>
#include <unistd.h>
#include "sharedvariable.h"
#include <QDebug>
#include "struct.h"
#include <QMutex>

namespace eink {

class VedioDecoderMp4Private {
	public:
	    VedioDecoderMp4Private(){
	        parseState = true;
	        pFormatCtx = NULL;
	        pCodecCtx =NULL;
	        pCodec = NULL;
	        pFrame = NULL;
	        pFrameRGB = NULL;
	        packet =NULL;
	        isDropFrameSeek = false;
	        dropFrameNum = 0;
	    }

		AVFormatContext *pFormatCtx;
		AVCodecContext *pCodecCtx;
		AVCodec *pCodec;
		AVFrame *pFrame;
		AVFrame *pFrameRGB;
		AVPacket *packet;
		uint8_t *out_buffer;
		struct SwsContext *img_convert_ctx;

		int videoStream;
		int audioIndex;
		int subtitleIndex;
		int numBytes;
		int got_picture;
		std::string filePath;
		bool parseState;

		AVRational time_base;
		int dropFrameNum;
		bool isDropFrameSeek;
};

bool caseInsensitiveLessThan(const Vedio_Image_ST &s1, const Vedio_Image_ST &s2)
{
    return s1.playTimePos < s2.playTimePos;
}

VedioDecoderMp4::VedioDecoderMp4(QObject *parent)
	: QObject(parent),d(new VedioDecoderMp4Private)
{
    av_register_all();
}

VedioDecoderMp4::~VedioDecoderMp4()
{
	if(d){
		delete d;
		d = NULL;
	}
}

void VedioDecoderMp4::ResetEvn()
{
    d->videoStream = -1;
    d->audioIndex = -1;
    d->subtitleIndex = -1;
    d->parseState = true;
    d->dropFrameNum = 0;
}

void VedioDecoderMp4::startParse()
{
    ResetEvn();

	printf("VedioDecoderMp4::startParse()!!!!!!!!!!\n");

	openVedioFile();
}

void VedioDecoderMp4::play()
{
    startParse();
}

void VedioDecoderMp4::stop()
{
    d->parseState = false;

    sws_freeContext(d->img_convert_ctx);
    d->img_convert_ctx = NULL;

    av_frame_free(&d->pFrame);
    d->pFrame = NULL;

    av_frame_free(&d->pFrameRGB);
    d->pFrameRGB = NULL;

    avcodec_close(d->pCodecCtx);

    avformat_close_input(&d->pFormatCtx);
    avformat_free_context(d->pFormatCtx);
    d->pFormatCtx = NULL;

    av_free_packet(d->packet);
    d->packet = NULL;

    if(d->out_buffer)
        delete d->out_buffer;
    d->out_buffer = nullptr;

}

void VedioDecoderMp4::suspendPlay()
{
}

void VedioDecoderMp4::keepOnPlay()
{
}

void VedioDecoderMp4::adjustPlayTiemPos(int )
{
}

const char *VedioDecoderMp4::getVedioPlayerType()
{
    return PLAYER_MSG[MP4PLAYER].c_str();
}

void VedioDecoderMp4::setFilePath(const std::string &dir)
{
	d->filePath = dir;
}

void VedioDecoderMp4::openVedioFile()
{
    d->pFormatCtx = avformat_alloc_context(); //打开并且初始化输入流

	int ret = avformat_open_input(&d->pFormatCtx, d->filePath.c_str(), NULL, NULL);
	if(ret != 0) {
		printf("open vedio file defeated!!!!\n");
		return;
	}

	/* Output info */
	printf("-------------File Information-------------\n");
	av_dump_format(d->pFormatCtx, 0,d->filePath.c_str(), 0);
	printf("------------------------------------------\n");

	findVedionStreamType();
}

void VedioDecoderMp4::findVedionStreamType()
{
    //循环查找视频中包含的流信息，直到找到视频类型的流
	//便将其记录下来 保存到videoStream变量中
	//这里我们现在只处理视频流，音频流先不管他

	int ret = avformat_find_stream_info(d->pFormatCtx, NULL);
	if(ret < 0 ) {
        printf("VedioDecoderMp4::findVedionStreamType() error info!!\n");
	    avformat_close_input(&d->pFormatCtx);
	    return;
	}

	for (int i = 0; i < d->pFormatCtx->nb_streams; i++) {
		if (d->pFormatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
			d->videoStream = i;
			printf("VedioDecoderMp4 Find video stream.\n");
		} else if(d->pFormatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
			d->audioIndex = i;
			printf("VedioDecoderMp4 Find audio stream.\n");
		} else if(d->pFormatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_SUBTITLE){
			d->subtitleIndex = i;
			printf("VedioDecoderMp4 Find subtitle stream.");
		}
	}

	//如果videoStream为-1 说明没有找到视频流
	if (d->videoStream == -1) {
		printf("Didn't find a video stream!!\n");
		return;
	}

	AVStream *stream = d->pFormatCtx->streams[d->videoStream];
    int frame_rate = stream->avg_frame_rate.num / stream->avg_frame_rate.den;//每秒多少帧
    frameRate_GL = 1000 / frame_rate;

    d->time_base = stream->time_base;
    qDebug() << "VedioDecoderMp4::findVedionStreamType() time_base num=" << d->time_base.num
            << d->time_base.den;
    qDebug() << "VedioDecoderMp4::findVedionStreamType() vedio frame rate==" << frame_rate;

	createDecompiler();
}

void VedioDecoderMp4::createDecompiler()
{
    //查找解码器
    if(!d->pCodecCtx)
        d->pCodecCtx = avcodec_alloc_context3(NULL);

    int ret = avcodec_parameters_to_context(d->pCodecCtx,
                         d->pFormatCtx->streams[d->videoStream]->codecpar);
    if(ret < 0) {
        printf("find avcodec faile!!!\n");
        return;
    }

    //解码方式
    d->pCodec = avcodec_find_decoder(d->pCodecCtx->codec_id);

	if (d->pCodec == NULL){
		printf("Codec not found\n");
		avformat_close_input(&d->pFormatCtx);
		return;
	}

	//打开解码器
	if (avcodec_open2(d->pCodecCtx, d->pCodec, NULL) < 0) {
		printf("Could not open codec\n");
		avcodec_free_context(&d->pCodecCtx);
		avformat_close_input(&d->pFormatCtx);
		return;
	}

	readFrameData();
}

void VedioDecoderMp4::decoderVedio()
{
        /* Output info */
	printf("-------------File Information-------------\n");
	av_dump_format(d->pFormatCtx, 0,d->filePath.c_str(), 0);
	printf("------------------------------------------\n");

	d->pFrame = av_frame_alloc();
        d->pFrameRGB = av_frame_alloc();
}

void VedioDecoderMp4::readFrameData()
{
	printf("VedioDecoderMp4::readFrameData() and parse file start!\n");
	if(!d->pFrame)
	    d->pFrame = av_frame_alloc();
	if(!d->pFrameRGB)
        d->pFrameRGB = av_frame_alloc();

    int picWidth = 0; //playUIWidth;
    int picHeight = 0; //playUIHeight
    qDebug() << "VedioDecoderMp4::readFrameData() srcWidth=" << d->pCodecCtx->width
                << "playUIWidth ==" << playUIWidth_GL;
    if( d->pCodecCtx->width > playUIWidth_GL)
        picWidth = playUIWidth_GL;
    else
        picWidth = d->pCodecCtx->width;
    picWidth = picWidth - (picWidth % 16);
    picHeight = picWidth * d->pCodecCtx->height / d->pCodecCtx->width;
    if(picHeight > playUIHeight_GL)
        picHeight = playUIHeight_GL;

	//设置格式转换后的 宽和高
    d->img_convert_ctx = sws_getContext(d->pCodecCtx->width, d->pCodecCtx->height,
            d->pCodecCtx->pix_fmt, picWidth, picHeight,
            AV_PIX_FMT_RGB32, SWS_BICUBIC, NULL, NULL, NULL);

    //一张图片需要的内存空间
    d->numBytes = avpicture_get_size(AV_PIX_FMT_BGR32, picWidth,picHeight);

    d->out_buffer = (uchar *) malloc(d->numBytes * sizeof(uchar));

    avpicture_fill((AVPicture *) d->pFrameRGB, d->out_buffer, AV_PIX_FMT_BGR32,
            picWidth, picHeight);

    d->packet = (AVPacket *) malloc(sizeof(AVPacket)); //分配一个packet
//下面两种办法分配空间都可以
#if 1
	av_init_packet(d->packet);
#else
    int y_size = d->pCodecCtx->width * d->pCodecCtx->height;
    av_new_packet(d->packet, y_size); //分配packet的数据
#endif

    while (d->parseState)
    {
        //av_read_frame读取的是一帧视频，并存入一个AVPacket的结构中
        if (av_read_frame(d->pFormatCtx, d->packet) < 0)
        {
            qDebug() << "VedioDecoderMp4::readFrameData(),parse file is finished,size="
                        << vedioImageQueue_GL.size();
            break; //这里认为视频读取完了
        }

		//视频里面的数据是经过编码压缩的，因此这里我们需要将其解码
        if (d->packet->stream_index == d->videoStream) {
            int ret = avcodec_decode_video2(d->pCodecCtx, d->pFrame, &d->got_picture,d->packet);
            if (ret <= 0) {
                printf("decode error!!!!");
                continue ;
            }
            //qDebug() << "@@@@@@@@@@@@@@@@@@@@@@@@@@@2==" << d->packet->size << ret;
            parseFrameToImage(picWidth,picHeight);
        }
        av_free_packet(d->packet);
    }

    isVedioPacketReadFinished_GL = true;
}

void VedioDecoderMp4::parseFrameToImage(int picWidth,int picHeight)
{
    //将得到的YUV420数据转换成RGB格式
    if (d->got_picture) {
        sws_scale(d->img_convert_ctx,
                (uint8_t const * const *) d->pFrame->data,
                d->pFrame->linesize, 0, d->pCodecCtx->height, d->pFrameRGB->data,
                d->pFrameRGB->linesize);

        //1.把解析得到的视频图片发送出去，或2.存放在全局变量中
        //当前采用第2种方法
        QImage Img((uchar *)d->out_buffer, picWidth,
            picHeight, QImage::Format_RGB32);

        Vedio_Image_ST temp;
        temp.image = Img.copy();
        temp.playTimePos = d->pFrame->pts * av_q2d(d->time_base) * 1000;
        QMutex mutex;
        mutex.lock();
        vedioImageQueue_GL.push_back(temp);
        //qSort(vedioImageQueue_GL.begin(),vedioImageQueue_GL.end(),caseInsensitiveLessThan);
        mutex.unlock();
        //qDebug() << "6566666666666666666666666666666===" << vedioImageQueue_GL.size();
        //qDebug() << "@@@@@@@@@@@@@@@@av_d->pFrame=====" << temp.playTimePos;

    } else {
        qDebug() << "d->packet->pts=====" << d->packet->pts * av_q2d(d->time_base) * 1000
                   << "d->packet->dts" << d->packet->dts * av_q2d(d->time_base) * 1000;
	    d->dropFrameNum += 1;
    }
}

void VedioDecoderMp4::determinePicType()
{
    if( AV_PICTURE_TYPE_I == d->pFrame->pict_type)
    {
        qDebug() << "<II>  pts=" << d->packet->pts * av_q2d(d->time_base) * 1000
                  << "dts="  << d->packet->dts * av_q2d(d->time_base) * 1000;
    }
    else if(AV_PICTURE_TYPE_P == d->pFrame->pict_type)
    {
        qDebug() << "<PP>  pts=" << d->packet->pts * av_q2d(d->time_base) * 1000
               << "dts="  << d->packet->dts * av_q2d(d->time_base) * 1000;
    }
    else if(AV_PICTURE_TYPE_B == d->pFrame->pict_type)
    {
        qDebug() << "<BB>  pts=" << d->packet->pts * av_q2d(d->time_base) * 1000
               << "dts="  << d->packet->dts * av_q2d(d->time_base) * 1000;
    }
}


void VedioDecoderMp4::setParseState(bool flag)
{
    printf("VedioDecoderMp4::setParseState  flag === %d\n",flag);
    d->parseState = flag;
}

void VedioDecoderMp4::dataDecompression()
{

}

void VedioDecoderMp4::translateCode()
{
}


}
