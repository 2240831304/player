#ifndef LABEL_H
#define LABEL_H

#include <QLabel>
namespace eink {
	class LineLabelPrivate;
	
	class LineLabel : public QLabel {
	public:
		LineLabel(int h, QWidget *parent = 0);
		LineLabel(int h, Qt::Orientation, QWidget *parent = 0);
		LineLabel(int h, Qt::Orientation, Qt::GlobalColor, QWidget *parent = 0);
		~LineLabel();
		
		void setOrientation(Qt::Orientation);
		void setColor(Qt::GlobalColor);
		void setColor(QColor);
		void setPenStyle(Qt::PenStyle);
		void setLength(int l);
		QSize sizeHint() const;

	protected:
		void paintEvent(QPaintEvent *);
		
	private:
		LineLabelPrivate *d;
	};

}

#endif
