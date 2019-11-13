
#ifndef PLAYMONITORTHREAD_H
#define PLAYMONITORTHREAD_H

#include <QObject>
#include <QThread>

class PlayerWidget;

namespace eink {

    class MonitorPrivate;
    class PlayMonitorThread : public QThread
    {
        Q_OBJECT
        public:
            PlayMonitorThread(QObject *parent = 0);
            ~PlayMonitorThread();

            void setPlayerWidget( PlayerWidget *);

        protected:
		    void run();

		private:
		    MonitorPrivate *d;
    };

}

#endif