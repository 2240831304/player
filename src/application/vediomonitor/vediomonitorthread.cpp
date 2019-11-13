
#include "vediomonitorthread.h"

namespace eink{

    VedioMonitorThread::VedioMonitorThread(QObject *parent )
        : QThread(parent)
    {
    }

    VedioMonitorThread::~VedioMonitorThread()
    {
    }

    void VedioMonitorThread::run()
    {
        grabType->start();
    }

    void VedioMonitorThread::setGrabType(BaseGrab *type)
    {
        grabType = type;
    }

}