
#include "capturethread.h"

namespace eink{

    CaptureThread::CaptureThread(QObject *parent )
        : QThread(parent)
    {
    }

    CaptureThread::~CaptureThread()
    {
    }

    void CaptureThread::run()
    {
        grabType->start();
    }

    void CaptureThread::setGrabType(BaseGrab *type)
    {
        grabType = type;
    }

}