#ifndef SHOWPICTUREWIDGET_H
#define SHOWPICTUREWIDGET_H

#include <QObject>
#include <QWidget>
#include <QHBoxLayout>
#include <QImage>

namespace eink {

	class ShowPictureWidgetPrivate;

	class ShowPictureWidget : public QWidget {
		Q_OBJECT
		public:
			ShowPictureWidget(QWidget *parent = 0);
			~ShowPictureWidget();

		public:
		    void setFilePath(const QString &);

		private:
		    void setupFace();
			void resetEvn();

		private slots:

		protected:

		private:
			ShowPictureWidgetPrivate *d;

	};


	class ShowPicturePlayWidget : public QWidget {
		Q_OBJECT
		public:
			ShowPicturePlayWidget(QWidget *parent = 0);
			~ShowPicturePlayWidget();

		protected:
			void paintEvent(QPaintEvent *);
            void resizeEvent (QResizeEvent *);

		private:

	};

}

#endif
