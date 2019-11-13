#include "label.h"
#include <QtGui>
#include <QScreen>

namespace eink {

	class LineLabelPrivate {
	public:
		int height;
		Qt::PenStyle style;
		Qt::Orientation orientation;
		QColor color;
	};
	
	LineLabel::LineLabel(int height, QWidget *parent)
		:QLabel(parent)
		,d(new LineLabelPrivate)
	{
		d->height = height;
		d->orientation = Qt::Horizontal;
		d->color = Qt::black;
		d->style = Qt::SolidLine; 
		setFixedHeight(height);
	}

	LineLabel::LineLabel(int height, Qt::Orientation o, QWidget *parent)
		:QLabel(parent)
		,d(new LineLabelPrivate)
	{
		if (o == Qt::Horizontal)
			setFixedHeight(height);
		else
			setFixedWidth(height);

		d->height = height;
		d->color = Qt::black;
		d->orientation = o;
		d->style = Qt::SolidLine; 
	}
	
	LineLabel::LineLabel(int height, Qt::Orientation o, Qt::GlobalColor color, QWidget *parent)
		:QLabel(parent)
		,d(new LineLabelPrivate)
	{
		if (o == Qt::Horizontal)
			setFixedHeight(height);
		else
			setFixedWidth(height);

		d->height = height;
		d->color = color;
		d->orientation = o;
		d->style = Qt::SolidLine; 
	}

	LineLabel::~LineLabel() 
	{
		if (d)
			delete d;
	}

	void LineLabel::setOrientation(Qt::Orientation o)
	{
		d->orientation = o;
	}

	void LineLabel::setPenStyle(Qt::PenStyle style)
	{
		d->style = style;
		update();
	}

	void LineLabel::setLength(int l)
	{
		if (d->orientation == Qt::Horizontal) {
			setFixedWidth(l);
		} else {
			setFixedHeight(l);
		}
	}

	void LineLabel::setColor(Qt::GlobalColor c)
	{
		d->color = c;
	}

	void LineLabel::setColor(QColor c)
	{
		d->color = c;
	}

	QSize LineLabel::sizeHint() const 
	{
//		if (d->orientation == Qt::Horizontal)
//			return QSize(QScreen::instance()->width(), d->height);
//		else
			return QSize(d->height, height());
	}

	void LineLabel::paintEvent(QPaintEvent *)
	{
		QPainter p(this);
		QPen pen(d->color);
		pen.setStyle(d->style);
		pen.setWidth(d->height);
		p.setPen(pen);
		int diff = d->height / 2;
		if (d->orientation == Qt::Horizontal)
			p.drawLine(QPoint(0, diff), QPoint(width(), diff));
		else
			p.drawLine(QPoint(diff, 0), QPoint(diff, height()));
	}

}

