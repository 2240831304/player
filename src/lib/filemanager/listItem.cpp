
#include "listItem.h"
#include <QDebug>
#include <QFileInfo>
#include <QPainter>
#include <QRect>
#include <QStandardItem>

ListItemDelegate::ListItemDelegate(QObject *parent)
    : QItemDelegate(parent)
{
}


ListItemDelegate::~ListItemDelegate()
{
}

void ListItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                             const QModelIndex &index) const
{
    QString fileName = index.data(1).toString();
    QFileInfo file(fileName);

    //const QStandardItemModel *model = dynamic_cast<const QStandardItemModel *> (index.model());
    //QStandardItem *item = model->item(index.row(),index.column());

    QRect rect =  option.rect;
    //QBrush brush = item->background();
    //painter->fillRect(rect,brush);

    if(option.state.testFlag(QStyle::State_Selected)){
        painter->fillRect(rect,QBrush(Qt::red));
    }

    //qDebug() << "ListItemDelegate::paint filename == " << file.fileName();

    painter->drawText(rect,file.fileName(), QTextOption(Qt::AlignLeft));

}
