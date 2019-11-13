#ifndef PLAYWIDGET_H
#define PLAYWIDGET_H

#include <QObject>
#include <QWidget>
#include <QHBoxLayout>
#include <QImage>

namespace eink {

	class PlayWidgetPrivate;

	class PlayWidget : public QWidget {
		Q_OBJECT
		public:
			PlayWidget(QWidget *parent = 0);
			~PlayWidget();

		public:
		    void startPlay();
		    bool getPlayState();
		    void restartPlay();

		private:
			void setupFace();
			QHBoxLayout *baseOperate();

			void playFinished();


		public slots:
		    void stopPlaySot();

		private slots:

			void continuePlaySlot();
			void timerSlot();

		protected:
            void timerEvent(QTimerEvent *);


		private:
			PlayWidgetPrivate *d;

	};


	class DrawImageWidget : public QWidget {
		Q_OBJECT
		public:
			DrawImageWidget(QWidget *parent = 0);
			~DrawImageWidget();

			void setImage(const QImage &);
			void setPlayFinishedState(bool);
			void setPlayTime();

		protected:
			void paintEvent(QPaintEvent *event);

            void resizeEvent (QResizeEvent *event);


		private:
			QImage mImage;
			int drawImageNum;
			bool isPlayFinished;

	};

}

#endif
