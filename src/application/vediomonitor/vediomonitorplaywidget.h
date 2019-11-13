

#include <QObject>
#include <QWidget>
#include <QImage>

class VedioMonitorPlayWidget : public QWidget
{
    Q_OBJECT
    public:
        VedioMonitorPlayWidget(QWidget *parent = 0);
        ~VedioMonitorPlayWidget();

    public:
        void setImage(QImage );


    protected:
        void paintEvent(QPaintEvent *);
        void resizeEvent(QResizeEvent *);

    private:
        QImage image;

};

