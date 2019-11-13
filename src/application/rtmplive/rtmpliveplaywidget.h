

#include <QObject>
#include <QWidget>
#include <QImage>

class RtmplivePlayWidget : public QWidget
{
    Q_OBJECT
    public:
        RtmplivePlayWidget(QWidget *parent = 0);
        ~RtmplivePlayWidget();

    public:
        void setImage(QImage );


    protected:
        void paintEvent(QPaintEvent *);
        void resizeEvent(QResizeEvent *);

    private:
        QImage image;

};

