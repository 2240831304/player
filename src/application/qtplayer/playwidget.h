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
		    void stopPlay();
		    bool getPlayState();
		    void setFilePath(const QString &);

		private:
			void setupFace();
			void playFinished();
			void suspendPlay();
			void continuePlay();
			void resetEvn();

		private slots:
			void timerSlot();
			void playStateClickedSlot(bool);

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

			void setImage(QImage );

		protected:
			void paintEvent(QPaintEvent *);
            void resizeEvent (QResizeEvent *event);

		private:
			QImage mImage;
			int drawImageNum;

	};

}

#endif
