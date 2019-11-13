
#ifndef PLAYCONTROLWIDGET_H
#define PLAYCONTROLWIDGET_H

#include <QWidget>
#include <QObject>

namespace eink{

    class PlayControlWidgetPrivate;

    class PlayControlWidget : public QWidget
    {
        Q_OBJECT
        public:
            PlayControlWidget(QWidget *parent = 0);
            ~PlayControlWidget();

        public:
            void setPlayState(bool);
            void setFileTotleTime(int);
            void setCurrentPlayTime(int);
            void setCurrentPlayTimeMillisecond(int);

        private:
            void setupUI();

        private slots:
            void playStateClickedSlot(bool );

        signals:
            void playStateClickedSig(bool);
            void nextPlaySig();
            void lastPlaySig();
            void changePlayTimePos(int );


        private:
            PlayControlWidgetPrivate *d;
    };

}

#endif