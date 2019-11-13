
#ifndef RTMPLIVETHREAD_H
#define RTMPLIVETHREAD_H

#include <QObject>
#include <QThread>
#include "basegrab.h"

namespace eink {

    class RtmpliveThread : public QThread
    {
        Q_OBJECT
        public:
            RtmpliveThread(QObject *parent = 0);
            ~RtmpliveThread();

            void setGrabType(BaseGrab *);

        protected:
            void run();

        private:
            BaseGrab *grabType;
    };

}

#endif