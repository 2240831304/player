
#include "recordsound.h"
#include <QDebug>
#include "packetlist.h"

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

static uint8_t  *audio_buf;
static int  audio_buf_size;
static int  audio_buf_index;


namespace eink {

RecordSound *RecordSound::StaticObject = NULL;

class RecordSoundPrivate {
public:
    AVFormatContext *pFormatCtx;
    AVCodecContext *pCodecCtx;
    AVCodec *pCodec;

    int audioIndex; //音频解析
    int subtitleIndex;
    std::string filePath;
    SDL_AudioSpec wanted_spec;
    SDL_AudioSpec spec;
    AVFrame wanted_frame;
    bool workState;
    PacketList audioq;
    int data_size;
    SwrContext *swr_ctx;

};


RecordSound::RecordSound()
    :d(new RecordSoundPrivate)
{
    packet_queue_init(&d->audioq);
}

RecordSound::~RecordSound()
{
    if(d){
        delete d;
        d = NULL;
    }
}

void RecordSound::InitEvn()
{
    //SDL初始化
    if (SDL_Init(SDL_INIT_AUDIO | SDL_INIT_TIMER ))
    {
        fprintf(stderr,"RecordSound::InitEvn() Couldn't init SDL:%s\n", SDL_GetError());
        return;
    }

    d->audioIndex = -1;
    d->data_size = 0;
    d->workState = true;
    audio_buf = NULL;
    audio_buf_size = 0;
    audio_buf_index = 0;
}

void RecordSound::freeMemory()
{
    swr_free(&d->swr_ctx);
}


void RecordSound::setFilePath(const std::string &)
{

}

void RecordSound::setMicrophoneTool(const std::string &)
{

}

void RecordSound::setMicrophoneDevice(const std::string &)
{

}

void RecordSound::setAudioPlayState(bool flag)
{
    d->workState = flag;
}

bool RecordSound::getAudioPlayState()
{
    return d->workState;
}

void RecordSound::run()
{
    startParse();
}

void RecordSound::startParse()
{
    StaticObject = this;

    InitEvn();

    d->pFormatCtx = avformat_alloc_context(); //打开并且初始化输入流

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

    AVInputFormat *ifmt = av_find_input_format("alsa");
    if(ifmt == NULL) {
        qDebug() << "RecordSound::startParse() no such audio device!! ";
        return;
    }

    int ret = avformat_open_input(&d->pFormatCtx,"plughw:0,0",ifmt,NULL);
    if(ret != 0){
        printf("RecordSound::startParse() Couldn't open input stream.\n");
        return;
    }
    av_dict_free(&options);

    findAudioStreamType();
}

void RecordSound::findAudioStreamType()
{
    int ret = avformat_find_stream_info(d->pFormatCtx, NULL);
    if( ret < 0 ) {
        printf("RecordSound::findVedionStreamType() error info!!\n");
        avformat_close_input(&d->pFormatCtx);
        return;
    }

    /* Output info */
    printf("-------------File Information-------------\n");
    av_dump_format(d->pFormatCtx, 0,0, 0);
    printf("------------------------------------------\n");

    for (int i = 0; i < d->pFormatCtx->nb_streams; i++) {
        if(d->pFormatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
            d->audioIndex = i;
            printf("RecordSound Find audio stream.\n");
        } else if(d->pFormatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_SUBTITLE){
            d->subtitleIndex = i;
            printf("RecordSound Find subtitle stream.");
        }
    }

    if (d->audioIndex == -1) {
        printf("AudioFileDecoderMp3:: Didn't find a audio stream!!\n");
        return;
    }

    AVStream *stream = d->pFormatCtx->streams[d->audioIndex];
    AVRational time_base = stream->time_base;
    qDebug() << "RecordSound::findVedionStreamType time_base num=" << time_base.num
        << time_base.den;

    openDecompiler();
}

void RecordSound::openDecompiler()
{
    //查找解码器
    d->pCodecCtx = avcodec_alloc_context3(NULL);
    int ret = avcodec_parameters_to_context(d->pCodecCtx,
                  d->pFormatCtx->streams[d->audioIndex]->codecpar);

    if( ret < 0) {
        printf("RecordSound:: find avcodec faile!!!\n");
        avformat_close_input(&d->pFormatCtx);
        return;
    }

    //解码方式
    d->pCodec = avcodec_find_decoder(d->pCodecCtx->codec_id);
    if (d->pCodec == NULL){
        printf("RecordSound:: Codec not found\n");
        avformat_close_input(&d->pFormatCtx);
        return;
    }

    //打开解码器
    if (avcodec_open2(d->pCodecCtx, d->pCodec, NULL) < 0) {
        printf("RecordSound:: Could not open codec\n");
        avcodec_free_context(&d->pCodecCtx);
        avformat_close_input(&d->pFormatCtx);
        return;
    }

    openAudioDevice();
}

void RecordSound::openAudioDevice()
{
    qDebug() << "RecordSound::readData() channels=" << d->pCodecCtx->channels
               << "sample_fmt" << d->pCodecCtx->sample_fmt;
    //设置音频信息, 用来打开音频设备。
    // SDL_LockAudio();
    d->wanted_spec.freq = d->pCodecCtx->sample_rate;
    d->wanted_spec.format = AUDIO_S16SYS;//d->pCodecCtx->sample_fmt;
    d->wanted_spec.channels = d->pCodecCtx->channels;        //通道数
    d->wanted_spec.silence = 0;    //设置静音值
    d->wanted_spec.samples = d->pCodecCtx->frame_size;
    d->wanted_spec.callback = audio_callback;
    d->wanted_spec.userdata = d->pCodecCtx;

    //wanted_spec:想要打开的,可以用SDL_OpenAudioDevice()代替

    if (SDL_OpenAudio(&d->wanted_spec, &d->spec) < 0)
    {
        fprintf(stderr, "RecordSound::openAudioDevice Couldn't open Audio:%s\n", SDL_GetError());
        return;
    } else {
        qDebug() <<  "\nRecordSound::openAudioDevice OPEN audio device succedd!!";
    }

    qDebug() << "RecordSound::OpenAudio() d->wanted_spec.channels== " <<
        d->wanted_spec.channels << "d->wanted_spec.freq=" << d->wanted_spec.freq
        << "d->wanted_spec.format==" << d->wanted_spec.format << "samples=" << d->pCodecCtx->frame_size;

    //设置参数，供解码时候用, swr_alloc_set_opts的in部分参数
    qDebug() << "RecordSound::OpenAudio() d->spec.channels== " <<
        d->spec.channels << "d->spec.freq=" << d->spec.freq << "d->spec.format==" << d->spec.format
        << "samples=" << d->spec.samples;

    d->wanted_frame.format = d->spec.format;
    d->wanted_frame.sample_rate = d->spec.freq;
    d->wanted_frame.channel_layout = av_get_default_channel_layout(d->spec.channels);
    d->wanted_frame.channels = d->spec.channels;

    read_device_Data();
}

void RecordSound::audio_callback(void *userdata, Uint8 *stream, int len)
{
    //qDebug() << "AudioFileDecoderMp3::audio_callback play audio buffer size = " << len;
    AVCodecContext *pcodec_ctx  = (AVCodecContext *)userdata;

     //每次进来先判断上次解析的数据是否都写入声音缓冲区，没有写入
     //都写入了就重新去解析新数据包，获取解析数据
     while( len > 0 && (SDL_GetAudioStatus() == SDL_AUDIO_PLAYING) )
     {
        //解析到audio_buf的数据全部拷贝到stream，继续解析
        if ((audio_buf_index >= audio_buf_size) ) {
            audio_buf_size = StaticObject->audio_decode_apcket(pcodec_ctx);
            if(audio_buf_size < 0) {
                SDL_memset(stream, 0, len);
                return;
            }
            audio_buf_index = 0;
        }

       //往设备声音缓存写入数据播放
       if(StaticObject->getAudioPlayState()) {
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

void RecordSound::read_device_Data()
{
    //分配一个packet
    AVPacket *packet = (AVPacket *) malloc(sizeof(AVPacket));
    av_init_packet(packet);

    //如果没有这个就放不出声音,0是不暂停，非零是暂停
    SDL_PauseAudio(0);

    //读一帧数据
    while(d->workState)
    {
        if(av_read_frame(d->pFormatCtx, packet) < 0) {
            qDebug() << "RecordSound::read_device_Data have no data!!!!";
            continue;
        }

        if (packet->stream_index == d->audioIndex)
        {
            packet_queue_put(&d->audioq, packet);
        }
        av_free_packet(packet);
    }
}

int RecordSound::audio_decode_apcket(AVCodecContext *aCodecCtx)
{
    static AVPacket pkt;
    static int audio_pkt_size = 0;
    static AVFrame frame;

    int len1 = 0;
    int data_size = 0;

    //每次进来先判断前面一个数据包有没有解析完成，如果没完成继续解析
    //解析完成了，再从数据队列中重新获取一个数据包解析
    //每次解析多少就返回多少解析的数据
    for(;;)
    {
        while(audio_pkt_size > 0) {
            int got_frame = 0;
            qDebug() << "@@@@@@@@@@@@@@@@@@@@@@@@@sssssssssssssssssssss";
            len1 = avcodec_decode_audio4(aCodecCtx, &frame, &got_frame, &pkt);
            qDebug() << "##########################ffffffffffffffffffffff";
            if(len1 < 0) {
                /* if error, skip frame */
                qDebug() << "AudioFileDecoderMp3::audio_decode_frame decode error!!";
                audio_pkt_size = 0;
                break;
            }

            // 得到该种格式数据一个buffer所需的空间大小
            data_size = av_samples_get_buffer_size(NULL,d->wanted_frame.channels,
                            aCodecCtx->frame_size,AV_SAMPLE_FMT_S16, 1);
            if(data_size <= 0) {
                qDebug() << "AudioFileDecoderMp3::audio_decode_frame data_size <= 0";
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
            //uint8_t *sample_buffer_R = frame.extended_data[1];//存放着右声道的数据

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
                                      aCodecCtx->sample_fmt,
                                      frame.sample_rate,
                                      0,
                                      NULL
                                 );

                //初始化,AV_SAMPLE_FMT_FLTP,
                if ( swr_init(swr_ctx) < 0)
                {
                    fprintf(stderr, "swr_init error!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
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

            //musicPlayTimePos_GL = pkt.pts * av_q2d(d->time_base) * 1000;

            /* We have data, return it and come back for more later */
            return data_size;
         }

        if(pkt.data)
            av_free_packet(&pkt);

        if(packet_queue_get(&d->audioq, &pkt) < 0) {
            qDebug() << "AudioFileDecoderMp3::audio_decode_frame packet_queue_get(&d->audioq, &pkt)< 0";
            return -1;
        }

        audio_pkt_size = pkt.size;
    }

}

void RecordSound::set_swr_convert_Info()
{
    // 得到该种格式数据一个buffer所需的空间大小
    d->data_size = av_samples_get_buffer_size(NULL,d->wanted_frame.channels,
                    d->pCodecCtx->frame_size,AV_SAMPLE_FMT_S16, 1);
    if(d->data_size <= 0) {
        qDebug() << "RecordSound::audio_decode_apcket data_size <= 0";
        return;
    }

    if(!audio_buf){
        audio_buf = (uint8_t *)malloc(d->data_size);
    } else {
        free(audio_buf);
        audio_buf = (uint8_t *)malloc(d->data_size);
    }

    int64_t in_channel_layout = av_get_default_channel_layout(d->pCodecCtx->channels);
    //qDebug() << "in_channel_layout=" << in_channel_layout << "frame.channel_layout=" << frame.channel_layout;
    //计算重采样的内存大小
    //av_rescale_rnd();
    d->swr_ctx = swr_alloc();
    //设置common parameters
    //2,3,4是output参数，4,5,6是input参数 d->wanted_frame.format,frame.channel_layout
    d->swr_ctx = swr_alloc_set_opts( 0,
                          d->wanted_frame.channel_layout,
                          AV_SAMPLE_FMT_S16,
                          d->wanted_frame.sample_rate,
                          in_channel_layout,
                          AV_SAMPLE_FMT_FLTP,
                          d->wanted_frame.sample_rate,
                          0,
                          NULL
                     );

    //初始化
    if ( swr_init(d->swr_ctx) < 0)
    {
        fprintf(stderr, "swr_init error!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
        return;
    }

    //int planar = av_sample_fmt_is_planar(AV_SAMPLE_FMT_S16);
    //int out_channels = av_get_channel_layout_nb_channels(d->wanted_frame.channel_layout);
    //qDebug() << "out_channels========== " << out_channels;
}



}
