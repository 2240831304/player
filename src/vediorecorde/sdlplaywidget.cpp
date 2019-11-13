
#include "sdlplaywidget.h"
#include <QDebug>
#include <iostream>

namespace eink{

    class SdlPlayWidgetPrivate{
        public:
           SDL_Window *screen;
           SDL_Renderer *sdlRenderer;
           SDL_Texture *sdlTexture;
           SDL_Rect rect;
           SDL_Event event;

           int width;
           int height;
    };



    SdlPlayWidget::SdlPlayWidget()
        : d(new SdlPlayWidgetPrivate)
    {
        av_register_all();
        avdevice_register_all();
    }

    SdlPlayWidget::~SdlPlayWidget()
    {
        if(d){
            delete d;
            d = NULL;
        }
    }

    void SdlPlayWidget::clearEvn()
    {
        SDL_DestroyTexture(d->sdlTexture);
    }

    void SdlPlayWidget::setSdlUISize(int UIwidth,int UIheight)
    {
        d->width = UIwidth;
        d->height = UIheight;
        d->rect.x = 0;
        d->rect.y = 0;
        d->rect.w = UIwidth;
        d->rect.h = UIheight;

        d->screen = NULL;
    }

    bool SdlPlayWidget::createPlayWindow(void *pWnd)
    {
        //SDL初始化
        if (SDL_Init(SDL_INIT_VIDEO))
        {
            qDebug() << "SdlPlayWidget::createSdlVedioPlayWidget Couldn't init SDL:" << SDL_GetError();
            return false;
        }
        char variable[64];
        sprintf(variable, "SDL_WINDOWID=0x%lx", pWnd);
        putenv(variable);

        qDebug() << "SdlPlayWidget::createSdlVedioPlayWidget nest other widget!!";

        d->screen = SDL_CreateWindowFrom(pWnd);
        if(!d->screen) {
            printf("SDL:SdlPlayWidget not create SDL video windows:%s\n",SDL_GetError());
            return false;
        }

   //SDL_SetWindowPosition(d->screen,SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED);
   //SDL_SetWindowFullscreen(d->screen,SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

        SDL_ShowWindow(d->screen);
        InitWindow();
    }

    void SdlPlayWidget::setWindowSize()
    {
        SDL_SetWindowSize(d->screen,d->width, d->height);
    }

    void SdlPlayWidget::test()
    {

        SDL_RWops *rwops = SDL_RWFromFile("/home/astron/Projects/player/resource/pic/homepage.jpg","rb");
        SDL_Surface *picture = IMG_LoadJPG_RW(rwops);
        if(!picture) {
            std::cout << SDL_GetError() << std::endl;
            return;
        }

       SDL_Texture *tex = SDL_CreateTextureFromSurface(d->sdlRenderer, picture);
       if(!tex) {
            std::cout << SDL_GetError() << std::endl;
            return;
       }
        SDL_RenderClear(d->sdlRenderer);
        SDL_RenderCopy(d->sdlRenderer, tex,&d->rect, &d->rect );
        SDL_RenderPresent(d->sdlRenderer);
        //SDL_Delay(3000);

    }



    bool SdlPlayWidget::createSdlVedioPlayWidget()
    {
        //SDL初始化
        if (SDL_Init(SDL_INIT_AUDIO | SDL_INIT_TIMER | SDL_INIT_TIMER))
        {
            qDebug() << "SdlPlayWidget::createSdlVedioPlayWidget Couldn't init SDL:" << SDL_GetError();
            return false;
        }

        qDebug() << "SdlPlayWidget::createSdlVedioPlayWidget Independent!!";
        d->screen = SDL_CreateWindow("RTMP Client Demo",
                  SDL_WINDOWPOS_CENTERED,
                  SDL_WINDOWPOS_CENTERED,
                  d->width, d->height,
                  SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

        if(!d->screen) {
            printf("SDL:SdlPlayWidget not create SDL video windows:%s\n",SDL_GetError());
            return false;
        }

        InitWindow();

        return true;
    }

    void SdlPlayWidget::InitWindow()
    {
       d->sdlRenderer = SDL_CreateRenderer(d->screen, -1,
            SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

        d->sdlTexture = SDL_CreateTexture(
            d->sdlRenderer,
            SDL_PIXELFORMAT_YV12,
            SDL_TEXTUREACCESS_STREAMING,
            d->width,
            d->height);
    }

    void SdlPlayWidget::playVedioData(AVFrame *pFrameYUV)
    {
        //SDL_UpdateTexture( d->sdlTexture, &d->rect,pFrameYUV->data[0], pFrameYUV->linesize[0]);
        SDL_UpdateYUVTexture(d->sdlTexture, &d->rect,pFrameYUV->data[0], pFrameYUV->linesize[0],
                                pFrameYUV->data[1], pFrameYUV->linesize[1],
                                pFrameYUV->data[2], pFrameYUV->linesize[2]);
        SDL_RenderClear( d->sdlRenderer );
        SDL_RenderCopy( d->sdlRenderer, d->sdlTexture, &d->rect, &d->rect );
        SDL_RenderPresent( d->sdlRenderer );
    }

    void SdlPlayWidget::playVedioDataArry(uint8_t *vedio_buffer_yuv,int num)
    {
        //SDL_UpdateYUVTexture
        SDL_UpdateTexture( d->sdlTexture, &d->rect,vedio_buffer_yuv, num);
        SDL_RenderClear( d->sdlRenderer );
        SDL_RenderCopy( d->sdlRenderer, d->sdlTexture, &d->rect, &d->rect );
        SDL_RenderPresent( d->sdlRenderer );
    }

    void SdlPlayWidget::closeWindow()
    {
        SDL_DestroyWindow(d->screen);
    }


}
