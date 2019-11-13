
#include "sdlplaywidget.h"
#include <QDebug>


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
    }

    void SdlPlayWidget::createSdlVedioPlayWidget(void *pWnd)
    {
        //SDL初始化
        if (SDL_Init(SDL_INIT_AUDIO | SDL_INIT_TIMER | SDL_INIT_TIMER))
        {
            fprintf(ERR_STREAM, "Couldn't init SDL:%s\n", SDL_GetError());
            return;
        }

        if(pWnd != NULL) {
            qDebug() << "SdlPlayWidget::createSdlVedioPlayWidget nest other widget!!";
            d->screen = SDL_CreateWindowFrom(pWnd);
        } else {
            qDebug() << "SdlPlayWidget::createSdlVedioPlayWidget Independent!!";
            d->screen = SDL_CreateWindow("RTSP Client Demo",
                      SDL_WINDOWPOS_UNDEFINED,
                      SDL_WINDOWPOS_UNDEFINED,
                      d->width, d->height,
                      SDL_WINDOW_RESIZABLE/* SDL_WINDOW_HIDDEN*/| SDL_WINDOW_OPENGL);
        }

        if(!d->screen) {
            printf("SDL: could not set video mode - exiting\n");
            return;
        }

        d->sdlRenderer = SDL_CreateRenderer(d->screen, -1,
            SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        InitWindow();
    }

    void SdlPlayWidget::InitWindow()
    {
        d->sdlTexture = SDL_CreateTexture(
            d->sdlRenderer,
            SDL_PIXELFORMAT_YV12,
            SDL_TEXTUREACCESS_STREAMING,
            d->width,
            d->height);
    }

    void SdlPlayWidget::playVedioData(AVFrame *pFrameYUV)
    {
        SDL_UpdateTexture( d->sdlTexture, &d->rect,pFrameYUV->data[0], pFrameYUV->linesize[0]);
        SDL_RenderClear( d->sdlRenderer );
        SDL_RenderCopy( d->sdlRenderer, d->sdlTexture, &d->rect, &d->rect );
        SDL_RenderPresent( d->sdlRenderer );
    }

    void SdlPlayWidget::exitPLayVedio()
    {
        SDL_PollEvent(&d->event);
        switch( d->event.type ) {
            case SDL_QUIT:
                SDL_Quit();
                exit(0);
                break;
            default:
                break;
        }
    }





}
