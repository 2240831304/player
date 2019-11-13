#include "audiofileinfo.h"
#include <stdio.h>

#define ERR_STREAM stderr
#include <QDebug>

extern "C"{
    #include <libavcodec/avcodec.h>
    #include <libavformat/avformat.h>
    #include <libavutil/pixfmt.h>
    #include <libswscale/swscale.h>
	#include <libavutil/fifo.h>
	#include <libavutil/mem.h>
	#include <libswresample/swresample.h>
	#include <libavutil/opt.h>
    #include <libavutil/channel_layout.h>
    #include <libavutil/samplefmt.h>
}


namespace eink {

	class AudioFileInfoPrivate {
		public :

		    AudioFileInfoPrivate()
		    {
	            pFormatCtx = NULL;
	            pCodecCtx =NULL;
	            pCodec = NULL;
		    }

		    AVFormatContext *pFormatCtx;
		    AVCodecContext *pCodecCtx;
		    AVCodec *pCodec;

		    int audioIndex; //音频解析
		    int subtitleIndex;

            std::string filePath;
            AVRational time_base;
            std::string fileAuthor;

	};

	AudioFileInfo::AudioFileInfo(QObject *parent)
		:QObject(parent),d(new AudioFileInfoPrivate)
	{
	    d->audioIndex = -1;
	    d->subtitleIndex = -1;
	    av_register_all();
	}

	AudioFileInfo::~AudioFileInfo()
	{
	    avcodec_close(d->pCodecCtx);
	    avformat_close_input(&d->pFormatCtx);

	    if(d){
	    	delete d;
	    	d = NULL;
	    }
	}

	void AudioFileInfo::InitEvn()
	{
	    d->fileAuthor.clear();
	}

    void AudioFileInfo::startParse()
    {
        qDebug() << "AudioFileInfo::startParse()##################";

        InitEvn();
        openFile();
    }

    void AudioFileInfo::setFilePath(const std::string &path)
    {
        d->filePath = path;
    }

	void AudioFileInfo::openFile()
	{
	    d->pFormatCtx = avformat_alloc_context(); //打开并且初始化输入流

	    int ret = avformat_open_input(&d->pFormatCtx, d->filePath.c_str(), NULL, NULL);
	    if(ret != 0) {
		    printf("open audio file defeated!!!!\n");
		    return;
		}

		findVedionStreamType();
	}


 	void AudioFileInfo::findVedionStreamType()
    {
        //循环查找视频中包含的流信息，直到找到视频类型的流
	    //便将其记录下来 保存到videoStream变量中
	    //这里我们现在只处理视频流，音频流先不管他

	    int ret = avformat_find_stream_info(d->pFormatCtx, NULL);
	    if(ret < 0 ) {
            printf("VedioDecoder::findVedionStreamType() error info!!\n");

	        avformat_close_input(&d->pFormatCtx);
	        return;
	    }

	    qDebug() << "AudioFileInfo::findVedionStreamType() vedio lenth=" << d->pFormatCtx->duration
	                << "nb_streams==" << d->pFormatCtx->nb_streams;

	    for (int i = 0; i < d->pFormatCtx->nb_streams; i++) {
		    if(d->pFormatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
			    d->audioIndex = i;
			    printf("AudioFileInfo Find audio stream.\n");
		    } else if(d->pFormatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_SUBTITLE){
			    d->subtitleIndex = i;
			    printf("AudioFileInfo Find subtitle stream.");
		    }
	    }

	    //如果videoStream为-1 说明没有找到视频流
	    if (d->audioIndex == -1) {
		    printf("Didn't find a audio stream!!\n");
		    return;
	    }


	    AVStream *stream = d->pFormatCtx->streams[d->audioIndex];

        d->time_base = stream->time_base;

        obtainFileInformation();

        //qDebug() << "AudioFileInfo::findVedionStreamType time_base num=" << d->time_base.num
        //        << d->time_base.den;

        //createDecompiler();
    }

    void AudioFileInfo::obtainFileInformation()
    {
        AVDictionaryEntry *t = NULL;
        while ((t = av_dict_get(d->pFormatCtx->metadata, "", t, AV_DICT_IGNORE_SUFFIX))) {
            printf("AudioFileInfo::key==%s,,value=%s\n", t->key, t->value);
        }

        //get author
        AVDictionaryEntry *avDictionaryEntry = NULL;
        if(d->pFormatCtx->metadata) {
            avDictionaryEntry = av_dict_get(d->pFormatCtx->metadata,"ARTIST",NULL,AV_DICT_IGNORE_SUFFIX);
            if(!avDictionaryEntry)
                avDictionaryEntry = av_dict_get(d->pFormatCtx->metadata,"artist",NULL,AV_DICT_IGNORE_SUFFIX);
        }
        if(avDictionaryEntry)
            d->fileAuthor = avDictionaryEntry->value;
    }

    //get file information
    std::string AudioFileInfo::getFileName()
    {
        return d->pFormatCtx->filename;
    }

    std::string AudioFileInfo::getFileAuthor()
    {
        return d->fileAuthor;
    }

    int AudioFileInfo::getFileTimeLenth()
    {
        return d->pFormatCtx->duration;
    }

    QImage AudioFileInfo::getFileCoverImage()
    {
        AVPacket pkt;
        av_init_packet(&pkt);
        pkt = d->pFormatCtx->streams[d->audioIndex]->attached_pic;
        QImage image = QImage::fromData((uchar*)pkt.data, pkt.size);
        av_free_packet(&pkt);

        return image;
    }

	void AudioFileInfo::createDecompiler()
	{
	    //查找解码器
        d->pCodecCtx = avcodec_alloc_context3(NULL);

        int ret = avcodec_parameters_to_context(d->pCodecCtx,
                        d->pFormatCtx->streams[d->audioIndex]->codecpar);
        if(ret < 0) {
            printf("find avcodec faile!!!\n");
            avformat_close_input(&d->pFormatCtx);
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
	}


}
