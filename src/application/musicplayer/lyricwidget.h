
#ifndef LYRICWIDGET_H
#define LYRICWIDGET_H

#include <QWidget>
#include <QObject>
#include <QImage>

namespace eink{

    class LyricWidgetPrivate;
    class LyricWidget : public QWidget
    {
        Q_OBJECT
        public:
            LyricWidget(QWidget *parent = 0);
            ~LyricWidget();

        public:
            void startPlayLyric();
            void setBackPicture(const QImage &);
            void setFileName(const QString &);
            void setShowTimePos(int);
            void setModelShowTimePos(int );

        private:
            void InitEvn();
            void setupUI();
            void createUI();
            void makeUI();
            void parseLyricFile();
            void showLyricContent();
            void addItemData(const QString &);
            void showLyricText();

        public slots:
            void autoScrollSlot(int );

        protected:
            void paintEvent(QPaintEvent *);

        private:
            LyricWidgetPrivate *d;
    };

}

#endif