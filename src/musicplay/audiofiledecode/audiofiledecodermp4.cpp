#include "audiofiledecodermp4.h"
#include <stdio.h>
#include <QQueue>
#include "sharedvariable.h"

#include "signalplugin.h"
#include "systempath.h"
#include <QPluginLoader>

#define AVCODE_MAX_AUDIO_FRAME_SIZE   192000 //1 second of 48khz 32bit audio
#define SDL_AUDIO_BUFFER_SIZE   1024
#define ERR_STREAM stderr
#define OUT_SAMPLE_RATE 44100

#include <QDebug>
#include <QMutex>
#include <QThread>

#include "packetqueue.h"
#include <QTime>
#include <QDateTime>

static uint8_t  *audio_buf;
static int  audio_buf_size;
static int  audio_buf_index;

namespace eink {

	class AudioDecoderPrivate {
		public :

		    AudioDecoderPrivate()
		    {
	            pFormatCtx = NULL;
	            pCodecCtx =NULL;
	            pCodec = NULL;
	            pFrame = NULL;
	            pFrameRGB = NULL;
	            packet =NULL;
	            isRuning = true;
	            isAudioPacketReadFinished = false;
	            isPLayFinished = false;
	            isStartToPlay = false;
		    }

		    AVFormatContext *pFormatCtx;
		    AVCodecContext *pCodecCtx;
		    AVCodec *pCodec;
		    AVFrame *pFrame;
		    AVFrame *pFrameRGB;
		    AVPacket *packet;

		    int audioIndex; //音频解析
		    int subtitleIndex;

		    /* 在用SDL_OpenAudio()打开音频设备的时候需要这两个参数*/
 	        /* wanted_spec是我们期望设置的属性，spec是系统最终接受的参数 */
 	        /* 我们需要检查系统接受的参数是否正确 */
 	        SDL_AudioSpec wanted_spec;
            SDL_AudioSpec spec;
            quint32 audioDeviceFormat;  // audio device sample format
            AVFrame wanted_frame;

            std::string filePath;

            PacketQueue audioq;
            bool isRuning;
            AVRational time_base;
            bool isAudioPacketReadFinished;
            bool isPLayFinished;
            bool isStartToPlay;

            PluginInterface *SigPlugin;
            QPluginLoader pluginLoader;

	};

    AudioFileDecoderMp4 *AudioFileDecoderMp4::CurMy = NULL;

    AudioFileDecoderMp4::AudioFileDecoderMp4(QObject *parent)
        :QObject(parent),d(new AudioDecoderPrivate)
	{
	    CurMy = this;

        //SDL初始化
        if (SDL_Init(SDL_INIT_AUDIO | SDL_INIT_TIMER | SDL_INIT_TIMER))
        {
            fprintf(ERR_STREAM, "Couldn't init SDL:%s\n", SDL_GetError());
            return;
        }

        packet_queue_init(&d->audioq);

        QString filePath = SystemPath::getPluginPath() + "/libsignalplugin.so";
        d->pluginLoader.setFileName(filePath);
        d->pluginLoader.load();
        QObject *plugin = d->pluginLoader.instance();

        if (plugin) {
            d->SigPlugin = qobject_cast<PluginInterface *>(plugin);
            //connect(plugin,SIGNAL(signalMessage(const QString &,const QString &)),
            //        this,SLOT(signalMessageSlot(const QString &,const QString &)));
            qDebug() << "AudioFileDecoderWav::AudioFileDecoderWav success test str=" << d->SigPlugin->getTestFunction();
        } else {
            qDebug() << "AudioFileDecoderWav::AudioFileDecoderWav  faile===================";
        }
	}

    AudioFileDecoderMp4::~AudioFileDecoderMp4()
	{
        qDebug() << "AudioFileDecoderMp4::~AudioFileDecoderMp4()<audioq = " << packet_queue_Size(&d->audioq);

	    if(d){
	    	delete d;
	    	d = NULL;
	    }
	}

    void AudioFileDecoderMp4::startParse()
    {
        qDebug() << "AudioFileDecoderMp4::startParse() @@@@@@@@@@@@";

        InitEvn();

        openFile();
    }

    void AudioFileDecoderMp4::InitEvn()
    {
        d->audioIndex = -1;
	    d->subtitleIndex = -1;
	    audio_buf = NULL;
	    audio_buf_size = 0;
	    audio_buf_index = 0;
        d->isRuning = true;
        d->isStartToPlay = false;
        d->isAudioPacketReadFinished = false;
        d->isPLayFinished = false;
    }

    void AudioFileDecoderMp4::openFile()
	{
	    d->pFormatCtx = avformat_alloc_context(); //打开并且初始化输入流

	    int ret = avformat_open_input(&d->pFormatCtx, d->filePath.c_str(), NULL, NULL);
	    if( ret != 0) {
		    printf("AudioFileDecoderMp4::openFile() faile!!!!\n");
		    return;
		}

		findVedionStreamType();
	}


    void AudioFileDecoderMp4::findVedionStreamType()
    {
        //循环查找视频中包含的流信息，直到找到视频类型的流
	    //便将其记录下来 保存到videoStream变量中
	    //这里我们现在只处理视频流，音频流先不管他

	    int ret = avformat_find_stream_info(d->pFormatCtx, NULL);
	    if( ret < 0 ) {
            printf("AudioFileDecoderMp4::findVedionStreamType() error info!!\n");

	        avformat_close_input(&d->pFormatCtx);
	        return;
	    }

        qDebug() << "AudioFileDecoderMp4::findVedionStreamType() vedio lenth=" << d->pFormatCtx->duration
                << "nb_streams==" << d->pFormatCtx->nb_streams;

	    for (int i = 0; i < d->pFormatCtx->nb_streams; i++) {
		    if(d->pFormatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
			    d->audioIndex = i;
                printf("AudioFileDecoderMp4 Find audio stream.\n");
		    } else if(d->pFormatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_SUBTITLE){
			    d->subtitleIndex = i;
                printf("AudioFileDecoderMp4 Find subtitle stream.");
		    }
	    }

	    if (d->audioIndex == -1) {
		    printf("AudioFileDecoderMp4:: Didn't find a audio stream!!\n");
		    return;
	    }


	    AVStream *stream = d->pFormatCtx->streams[d->audioIndex];
        //int frame_rate = stream->avg_frame_rate.num / stream->avg_frame_rate.den;
        //qDebug() << "AudioFileDecoderMp4::findVedionStreamType Audio frame rate==" << frame_rate;

        d->time_base = stream->time_base;
        qDebug() << "AudioFileDecoderMp4::findVedionStreamType time_base num=" << d->time_base.num
                << d->time_base.den;

        createDecompiler();
    }

    void AudioFileDecoderMp4::createDecompiler()
	{
	    //查找解码器
        d->pCodecCtx = avcodec_alloc_context3(NULL);

        int ret = avcodec_parameters_to_context(d->pCodecCtx,
                        d->pFormatCtx->streams[d->audioIndex]->codecpar);
        if( ret < 0) {
            printf("AudioFileDecoderMp4:: find avcodec faile!!!\n");
            avformat_close_input(&d->pFormatCtx);
            return;
        }

        //解码方式
        d->pCodec = avcodec_find_decoder(d->pCodecCtx->codec_id);

	    if (d->pCodec == NULL){
		    printf("AudioFileDecoderMp4:: Codec not found\n");
		    avformat_close_input(&d->pFormatCtx);
		    return;
	    }

	    //打开解码器
	    if (avcodec_open2(d->pCodecCtx, d->pCodec, NULL) < 0) {
		    printf("AudioFileDecoderMp4:: Could not open codec\n");
		    avcodec_free_context(&d->pCodecCtx);
		    avformat_close_input(&d->pFormatCtx);
		    return;
	    }

        //打开音频设备，开始读取数据播放
        openAudioDevice();
        readData();
	}

    void AudioFileDecoderMp4::openAudioDevice()
	{
        qDebug() << "AudioFileDecoderMp4::readData() channels=" << d->pCodecCtx->channels
	               << "sample_fmt" << d->pCodecCtx->sample_fmt;
        //设置音频信息, 用来打开音频设备。
        // SDL_LockAudio();
        d->wanted_spec.freq        = d->pCodecCtx->sample_rate;
        d->wanted_spec.format      = AUDIO_S16SYS;//d->pCodecCtx->sample_fmt;
        d->wanted_spec.channels    = d->pCodecCtx->channels;        //通道数
        d->wanted_spec.silence     = 0;    //设置静音值
        d->wanted_spec.samples     = d->pCodecCtx->frame_size;
        d->wanted_spec.callback    = audio_callback;
        d->wanted_spec.userdata    = d->pCodecCtx;

        //wanted_spec:想要打开的
        //spec: 实际打开的,可以不用这个，函数中直接用NULL，下面用到spec的用wanted_spec代替。
        //这里会开一个线程，调用callback。
        //SDL_OpenAudio->open_audio_device(开线程)->SDL_RunAudio->fill(指向callback函数）
        //可以用SDL_OpenAudioDevice()代替

        if (SDL_OpenAudio(&d->wanted_spec, &d->spec) < 0)
        {
            fprintf(ERR_STREAM, "Couldn't open Audio:%s\n", SDL_GetError());
            return;
        } else {
            qDebug() <<  "\nOPEN audio device succedd!!";
        }

        qDebug() << "AudioFileDecoderMp4::OpenAudio() d->wanted_spec.channels== " <<
                d->wanted_spec.channels << "d->wanted_spec.freq=" << d->wanted_spec.freq
                << "d->wanted_spec.format==" << d->wanted_spec.format << "samples=" << d->pCodecCtx->frame_size;
        //设置参数，供解码时候用, swr_alloc_set_opts的in部分参数
        qDebug() << "AudioFileDecoderMp4::OpenAudio() d->spec.channels== " <<
                d->spec.channels << "d->spec.freq=" << d->spec.freq << "d->spec.format==" << d->spec.format
                << "samples=" << d->spec.samples;

        d->wanted_frame.format         = d->spec.format;
        d->wanted_frame.sample_rate    = d->spec.freq;
        d->wanted_frame.channel_layout = av_get_default_channel_layout(d->spec.channels);
        d->wanted_frame.channels       = d->spec.channels;
    }

    void AudioFileDecoderMp4::readData()
    {
        //分配一个packet
        AVPacket *packet = (AVPacket *) malloc(sizeof(AVPacket));
        av_init_packet(packet);

        //读一帧数据
        while(!d->isAudioPacketReadFinished)
        {
            if(av_read_frame(d->pFormatCtx, packet) < 0){
                d->isAudioPacketReadFinished = true;
                break;
            }

            if (packet->stream_index == d->audioIndex)
            {
                packet_queue_put(&d->audioq, packet);
                if(!d->isStartToPlay) {
                    //如果没有这个就放不出声音,0是不暂停，非零是暂停
                    SDL_PauseAudio(0);
                    d->isStartToPlay = true;
                    isMusicStartPlay_GL = true;
                    qDebug() << "AudioFileDecoderMp4::readData() satrt play audio@@@@@@@@";
                }
            } else {
                av_free_packet(packet);
            }
        }

        qDebug() << "AudioFileDecoderMp4::readData packet finished!!!size="
                    << packet_queue_Size(&d->audioq) << d->isAudioPacketReadFinished;
	}


//注意userdata是前面的AVCodecContext.
//len的值常为2048,表示一次发送多少。
//audio_buf_size：一直为样本缓冲区的大小，wanted_spec.samples.（一般每次解码这么多，文件不同，这个值不同)
//audio_buf_index： 标记发送到哪里了。
//这个函数的工作模式是:
//1. 解码数据放到audio_buf, 大小放audio_buf_size。(audio_buf_size = audio_size;这句设置）
//2. 调用一次callback只能发送len个字节,而每次取回的解码数据可能比len大，一次发不完。
//3. 发不完的时候，会len == 0，不继续循环，退出函数，继续调用callback，进行下一次发送。
//4. 由于上次没发完，这次不取数据，发上次的剩余的，audio_buf_size标记发送到哪里了。
//5. 注意，callback每次一定要发且仅发len个数据，否则不会退出。
//如果没发够，缓冲区又没有了，就再取。发够了，就退出，留给下一个发，以此循环。
//三个变量设置为static就是为了保存上次数据，也可以用全局变量，但是感觉这样更好。
    void AudioFileDecoderMp4::audio_callback(void *userdata, Uint8 *stream, int len)
    {
        //qDebug() << "AudioFileDecoderMp4::audio_callback play audio buffer size = " << len;

//        if(CurMy->getPlayFinishedState()){
//            SDL_PauseAudio(1);
//            return;
//        }

        AVCodecContext *pcodec_ctx  = (AVCodecContext *)userdata;

         //每次进来先判断上次解析的数据是否都写入声音缓冲区，没有写入
         //都写入了就重新去解析新数据包，获取解析数据
         while( len > 0 && (SDL_GetAudioStatus() == SDL_AUDIO_PLAYING) )
         {
            //解析到audio_buf的数据全部拷贝到stream，继续解析
            if ((audio_buf_index >= audio_buf_size) && CurMy->getAudioDecoderState()) {
                audio_buf_size = CurMy->audio_decode_frame(pcodec_ctx);
                if(audio_buf_size < 0 && CurMy->getAudioDecoderState()) {
                    SDL_memset(stream, 0, len);
                    return;
                }
                audio_buf_index = 0;
            }

           //往设备声音缓存写入数据播放
           if(CurMy->getAudioDecoderState()) {
                //每次stream写满之后就重新出发回调函数
                int len1 = audio_buf_size - audio_buf_index;
                if (len1 > len)
                    len1 = len;
                memcpy(stream, (uint8_t *) audio_buf + audio_buf_index, len1);

                len -= len1;
                stream += len1;
                audio_buf_index += len1;
            }
         }
    }


    int AudioFileDecoderMp4::audio_decode_frame(AVCodecContext *aCodecCtx)
    {
        static AVPacket pkt;
        static int audio_pkt_size = 0;
        static AVFrame frame;

        int len1 = 0;
        int data_size = 0;

        //音频解析，播放完毕
        if( packet_queue_empty(&d->audioq) && d->isAudioPacketReadFinished ) {
            qDebug() << "AudioFileDecoderMp4::audio_decode_frame finished!!!!";
            SDL_PauseAudio(1);
            isMusicPlayFinished_GL = true;
            isAutoMusicPlayFinished_GL = true;
            d->isPLayFinished = true;
            d->isRuning = false;
            d->SigPlugin->sendMessage("Eink/System","PlayFinished");
            return -1;
        }

        //每次进来先判断前面一个数据包有没有解析完成，如果没完成继续解析
        //解析完成了，再从数据队列中重新获取一个数据包解析
        //每次解析多少就返回多少解析的数据
        for(;;)
        {
            while(audio_pkt_size > 0) {
                int got_frame = 0;
                len1 = avcodec_decode_audio4(aCodecCtx, &frame, &got_frame, &pkt);
                if(len1 < 0) {
                    /* if error, skip frame */
                    audio_pkt_size = 0;
                    break;
                }

                // 得到该种格式数据一个buffer所需的空间大小
                data_size = av_samples_get_buffer_size(NULL,d->wanted_frame.channels,
                                aCodecCtx->frame_size,AV_SAMPLE_FMT_S16, 1);
                if(data_size <= 0) {
                    /* No data yet, get more frames */
                    return -1;
                }

                if(!audio_buf){
                    audio_buf = (uint8_t *)malloc(data_size);
                } else {
                    free(audio_buf);
                    audio_buf = (uint8_t *)malloc(data_size);
                }

                //uint8_t *sample_buffer_L = frame.extended_data[0];//存放着左声道的数据
                //uint8_t *sample_buffer_R = frame.extended_data[1];//存放着右声道的数据</span></span>

                audio_pkt_size -= len1;

                if(got_frame) {
                     int64_t in_channel_layout = av_get_default_channel_layout(aCodecCtx->channels);
                    //qDebug() << "in_channel_layout=" << in_channel_layout << "frame.channel_layout=" << frame.channel_layout;
                    //计算重采样的内存大小
                    //av_rescale_rnd();
                    SwrContext *swr_ctx = swr_alloc();
                    //设置common parameters
                    //2,3,4是output参数，4,5,6是input参数 d->wanted_frame.format,frame.channel_layout
                    swr_ctx = swr_alloc_set_opts( 0,
                                          d->wanted_frame.channel_layout,
                                          AV_SAMPLE_FMT_S16,
                                          d->wanted_frame.sample_rate,
                                          in_channel_layout,
                                          AV_SAMPLE_FMT_FLTP,
                                          frame.sample_rate,
                                          0,
                                          NULL
                                     );

                    //初始化
                    if ( swr_init(swr_ctx) < 0)
                    {
                        fprintf(ERR_STREAM, "swr_init error!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
                        return -1;
                    }

                    //int planar = av_sample_fmt_is_planar(AV_SAMPLE_FMT_S16);
                    //int out_channels = av_get_channel_layout_nb_channels(d->wanted_frame.channel_layout);
                    //qDebug() << "out_channels========== " << out_channels;

                    //重采样，往解析缓存里面写数据
                    int parseRet = swr_convert(swr_ctx,
                                        &audio_buf,
                                        frame.nb_samples,
                                        (const uint8_t **)frame.data,
                                        frame.nb_samples
                                     );

                    swr_free(&swr_ctx);
                }

                musicPlayTimePos_GL = pkt.pts * av_q2d(d->time_base) * 1000;

                /* We have data, return it and come back for more later */
                return data_size;
             }

            if(pkt.data)
                av_free_packet(&pkt);

            if(packet_queue_get(&d->audioq, &pkt) < 0) {
                qDebug() << "AudioFileDecoderMp4::audio_decode_frame packet_queue_get(&d->audioq, &pkt) < 0";
                return -1;
            }

            audio_pkt_size = pkt.size;
        }

    }

    void AudioFileDecoderMp4::setAudioDecoderState(bool flag)
    {
        d->isRuning = flag;
        qDebug() << "AudioFileDecoderMp4::setAudioDecoderState flag=" << flag;
    }

    bool AudioFileDecoderMp4::getAudioDecoderState()
    {
        return d->isRuning;
    }

    void AudioFileDecoderMp4::freeMemory()
    {
    	av_frame_free(&d->pFrame);
	    avcodec_close(d->pCodecCtx);
	    avformat_close_input(&d->pFormatCtx);
	    av_free_packet(d->packet);

        if(audio_buf) {
            free(audio_buf);
            audio_buf = NULL;
        }
	    packet_queue_clear(&d->audioq);
    }

    int AudioFileDecoderMp4::getAudioPacketSize()
    {
        return packet_queue_Size(&d->audioq);
    }

    void AudioFileDecoderMp4::play()
    {
        d->isRuning = true;
        startParse();
    }

	void AudioFileDecoderMp4::stop()
	{
	    d->isRuning = false;
	    isMusicPlayFinished_GL = true;
        quitPlay();
        freeMemory();
        qDebug() << "AudioFileDecoderMp4::stop()!!!!!!!!!===";
	}

	void AudioFileDecoderMp4::suspendPlay()
	{
	    d->isRuning = false;
	    SDL_PauseAudio(1);
	}

	void AudioFileDecoderMp4::keepOnPlay()
	{
	    d->isRuning = true;
	    SDL_PauseAudio(0);
	}

	const char *AudioFileDecoderMp4::getAudioPlayerType()
	{
	    return AUDIO_MSG[FILE_MP4].c_str();
	}

	void AudioFileDecoderMp4::setFilePath(const std::string &path)
	{
	    d->filePath = path;
	}

	bool AudioFileDecoderMp4::getPlayFinishedState()
	{
	    return d->isPLayFinished;
	}

	void AudioFileDecoderMp4::quitPlay()
	{
        if(SDL_GetAudioStatus() == SDL_AUDIO_PLAYING) {
            SDL_PauseAudio(1);
            SDL_CloseAudio();
        } else if(SDL_GetAudioStatus() == SDL_AUDIO_PAUSED) {
            SDL_CloseAudio();
        }
	}

	void AudioFileDecoderMp4::adjustPlayTimePos(int timePos)
	{
	    d->isRuning = false;
	    if(SDL_GetAudioStatus() == SDL_AUDIO_PLAYING) {
	        SDL_PauseAudio(1);
	    }

	    musicPlayTimePos_GL = timePos;
	    avSeekPacket(timePos);
	}

	void AudioFileDecoderMp4::avSeekPacket(int timestamp)
    {
        int64_t posTime = timestamp / 1000;
        posTime =  posTime / av_q2d(d->time_base);
        int ret = av_seek_frame(d->pFormatCtx,d->audioIndex,posTime,AVSEEK_FLAG_BACKWARD);
        if(ret < 0) {
            qDebug() << "AudioFileDecoderWav::avSeekPacket failed!!!!!!!!!!";
            d->isRuning = true;
            SDL_PauseAudio(0);
            return;
        }

        //如果之前没有读取完数据包，则停止
        d->isAudioPacketReadFinished = true;

        //清空之前读取的数据包
        packet_queue_clear(&d->audioq);

        //分配一个packet
        AVPacket *packet = (AVPacket *) malloc(sizeof(AVPacket));
        av_init_packet(packet);

        d->isAudioPacketReadFinished = false;
        d->isStartToPlay = false;
        audio_buf_index = audio_buf_size = 0;

        //从新读取数据包
        while(!d->isAudioPacketReadFinished)
        {
            if(av_read_frame(d->pFormatCtx, packet) < 0){
                d->isAudioPacketReadFinished = true;
                break;
            }

            if (packet->stream_index == d->audioIndex)
            {
                packet_queue_put(&d->audioq, packet);
                //qDebug() << "@@@@@@@@@@@@@@@@@@@=" << packet->pts * av_q2d(d->time_base) * 1000;
                if(!d->isStartToPlay && packet_queue_Size(&d->audioq) > 5) {
                //如果没有这个就放不出声音,0是不暂停，非零是暂停
                    SDL_PauseAudio(0);
                    d->isStartToPlay = true;
                    isMusicStartPlay_GL = true;
                    isMusicPlayFinished_GL = false;
                    isAutoMusicPlayFinished_GL = false;
                    d->isPLayFinished = false;
                    d->isRuning = true;

                    d->SigPlugin->sendMessage("Eink/System","AdjustPlayRestart");
                    qDebug() << "AudioFileDecoderMp4::avSeekPacket satrt play audio!!";
                }
            } else {
                av_free_packet(packet);
            }
        }
        qDebug() << "AudioFileDecoderMp4::avSeekPacket finished ,,size="
                    << packet_queue_Size(&d->audioq);
	}

    void AudioFileDecoderMp4::createAudioFile()
    {

    #if 0
        static char *outfilename = "/home/astron/Desktop/12345.pcm";
        static FILE *outFile = fopen(outfilename, "wb");
        int data_size_size = av_get_bytes_per_sample(aCodecCtx->sample_fmt);
        if (data_size_size < 0) {
            /* This should not occur, checking just for paranoia */
            fprintf(stderr, "Failed to calculate data size\n");
             exit(1);
        }
        for (int i = 0; i < frame.nb_samples; i++)
            for (int ch = 0; ch < aCodecCtx->channels; ch++)
                fwrite(frame.data[ch] + data_size_size*i, 1, data_size_size, outFile);

        fseek(outFile, 0, SEEK_END);//设置流文件指针的位置,以SEEK_END为起点，偏移量是0,亦即SEEK_END
        data_size = ftell(outFile);//函数结果：当前文件流指针位置相对于文件起始位置的字节偏移量
        qDebug() << "55555555555555555=================" << data_size;
        fclose(outFile);
    #endif
    }

}
