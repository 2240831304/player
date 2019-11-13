
#ifndef LISTVIEW_H
#define LISTVIEW_H

#include <QObject>
#include <QListView>

namespace eink {

    class ListView : public QListView
    {
        Q_OBJECT
        public:
            ListView(QWidget *parent = 0);
            ~ListView();

        protected:
            void enterEvent (QEvent *);
            void leaveEvent (QEvent *);
            void focusInEvent ( QFocusEvent *);
            void focusOutEvent( QFocusEvent *);

            //void mouseMoveEvent ( QMouseEvent * );
            //void mousePressEvent ( QMouseEvent * );
            //void mouseReleaseEvent ( QMouseEvent * );
            //void paintEvent ( QPaintEvent *);
    };

}

#endif