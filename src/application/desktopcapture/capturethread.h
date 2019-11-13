
#ifndef CAPTURETHREAD_H
#define CAPTURETHREAD_H

#include <QObject>
#include <QThread>
#include "basegrab.h"

namespace eink {

    class CaptureThread : public QThread
    {
        Q_OBJECT
        public:
            CaptureThread(QObject *parent = 0);
            ~CaptureThread();

            void setGrabType(BaseGrab *);

        protected:
            void run();

        private:
            BaseGrab *grabType;
    };

}

#endif