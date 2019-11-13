
#ifndef VEDIOMONITORTHREAD_H
#define VEDIOMONITORTHREAD_H

#include <QObject>
#include <QThread>
#include "basegrab.h"

namespace eink {

    class VedioMonitorThread : public QThread
    {
        Q_OBJECT
        public:
            VedioMonitorThread(QObject *parent = 0);
            ~VedioMonitorThread();

            void setGrabType(BaseGrab *);

        protected:
            void run();

        private:
            BaseGrab *grabType;
    };

}

#endif