
#include "playmonitorthread.h"
#include "sharedvariable.h"
#include "playerwidget.h"

namespace eink{

    class  MonitorPrivate {
        public:

            PlayerWidget *playerWid;
            int timePos;
            bool isStart;
    };

    PlayMonitorThread::PlayMonitorThread(QObject *parent)
        : QThread(parent),d(new MonitorPrivate)
    {
    }

    PlayMonitorThread::~PlayMonitorThread()
    {
    }

    void PlayMonitorThread::setPlayerWidget( PlayerWidget *pt)
    {
        d->playerWid = pt;
        d->timePos = -1;
        d->isStart = false;
    }


    void PlayMonitorThread::run()
    {
        d->timePos = 0;
        d->isStart = false;

        while(1) {
            if(isMusicPlayFinished_GL) {
                break;
            }

            int time = musicPlayTimePos_GL / 1000;
            if( time != d->timePos ){
                d->timePos = time;
                d->playerWid->setCurPlayTimePos(musicPlayTimePos_GL);
            }

            if(d->isStart) {
                msleep(200);
            } else {
                if(isMusicStartPlay_GL)
                    d->isStart = true;
            }
        }
    }

}