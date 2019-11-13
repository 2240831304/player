#ifndef PROGRESSBAR_H
#define PROGRESSBAR_H

#include <QSlider>
#include <QMouseEvent>

namespace eink{

    class ProgressBar : public QSlider
    {
        Q_OBJECT
        public:
            ProgressBar(QWidget *parent = 0);
            ~ProgressBar();
            
        protected:
            void mousePressEvent(QMouseEvent *ev);
            void mouseReleaseEvent(QMouseEvent *ev);
            void mouseMoveEvent ( QMouseEvent * ev);
            bool eventFilter(QObject *obj, QEvent *event);

        signals:
            void changePlayTimePos(int );

        private:
            bool isMousePress;
    };
}

#endif