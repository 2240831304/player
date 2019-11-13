
#include "listview.h"


namespace eink {

     ListView::ListView(QWidget *parent)
        : QListView(parent)
     {
     }

     ListView::~ListView()
     {
     }

    void ListView::enterEvent (QEvent *)
    {
    }

    void ListView::leaveEvent (QEvent *)
    {
    }

     void ListView::focusInEvent ( QFocusEvent *)
     {
     }

     void ListView::focusOutEvent( QFocusEvent *)
     {
     }

//     void ListView::mouseMoveEvent ( QMouseEvent * )
//     {
//     }
//     void ListView::mousePressEvent ( QMouseEvent * )
//     {
//     }
//     void ListView::mouseReleaseEvent ( QMouseEvent * )
//     {
//     }

//    void ListView::paintEvent ( QPaintEvent *)
//    {
//    }


}