

#include <QItemDelegate>


class ItemDelegate : public QItemDelegate
{
    Q_OBJECT
    public:
        ItemDelegate(QObject *parent = 0);
        ~ItemDelegate();

    protected:
        void paint(QPainter *painter, const QStyleOptionViewItem &option,
                   const QModelIndex &index) const;
};
