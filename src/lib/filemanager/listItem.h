

#include <QItemDelegate>


class ListItemDelegate : public QItemDelegate
{
    Q_OBJECT
    public:
        ListItemDelegate(QObject *parent = 0);
        ~ListItemDelegate();

    protected:
        void paint(QPainter *painter, const QStyleOptionViewItem &option,
                   const QModelIndex &index) const;
};
