
#ifndef PLAYMANGER_H
#define PLAYMANGER_H

#include <QObject>

namespace eink {
	class PlayManagerPrivate;

	class PlayManager
	{
		public:
			PlayManager();
			~PlayManager();

			void setPlayFilePath(const QString &);
			void startPlay();
			void stopPlay();

		private:
			void init();
			void resetEvn();

		private:
			PlayManagerPrivate *d;
	};

}

#endif
