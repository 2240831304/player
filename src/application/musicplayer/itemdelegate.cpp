
#include "itemdelegate.h"
#include <QDebug>
#include <QFileInfo>
#include <QPainter>
#include <QRect>
#include <QStandardItem>

ItemDelegate::ItemDelegate(QObject *parent)
    : QItemDelegate(parent)
{
}


ItemDelegate::~ItemDelegate()
{
}

void ItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                             const QModelIndex &index) const
{
    QString str = index.data(1).toString();

    //painter->save();
    //const QStandardItemModel *model = dynamic_cast<const QStandardItemModel *> (index.model());
    //QStandardItem *item = model->item(index.row(),index.column());

    QRect rect =  option.rect;
    //QBrush brush = item->background();
    //painter->fillRect(rect,brush);

    if(option.state.testFlag(QStyle::State_Selected)){
        //painter->fillRect(rect,QBrush(Qt::red));
        painter->setPen(QPen(Qt::red));
        painter->setBrush(Qt::NoBrush);
        painter->drawText(rect,str, QTextOption(Qt::AlignHCenter));
        painter->setPen(QPen(Qt::black));
        //painter->restore();
    } else {
        painter->drawText(rect,str, QTextOption(Qt::AlignHCenter));
    }

}
