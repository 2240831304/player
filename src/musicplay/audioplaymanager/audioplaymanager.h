
#ifndef AUDIOPLAYMANGER_H
#define AUDIOPLAYMANGER_H

#include <QObject>

namespace eink {
	class AudioPlayManagerPrivate;

	class AudioPlayManager
	{
		public:
			AudioPlayManager();
			~AudioPlayManager();

			void setPlayFilePath(const QString &);
			void startPlay();
			void stopPlay();
			void suspendPlay();
			void keepOnPlay();
			void adjustPlayTime(int );

		private:
			void init();
			void resetEvn();

		private:
			AudioPlayManagerPrivate *d;
	};

}

#endif
