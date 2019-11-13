
#include "rtmplivethread.h"

namespace eink{

    RtmpliveThread::RtmpliveThread(QObject *parent )
        : QThread(parent)
    {
    }

    RtmpliveThread::~RtmpliveThread()
    {
    }

    void RtmpliveThread::run()
    {
        grabType->start();
    }

    void RtmpliveThread::setGrabType(BaseGrab *type)
    {
        grabType = type;
    }

}