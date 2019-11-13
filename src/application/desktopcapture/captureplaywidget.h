

#include <QObject>
#include <QWidget>
#include <QImage>

class CapturePlayWidget : public QWidget
{
    Q_OBJECT
    public:
        CapturePlayWidget(QWidget *parent = 0);
        ~CapturePlayWidget();

    public:
        void setImage(QImage );


    protected:
        void paintEvent(QPaintEvent *);
        void resizeEvent(QResizeEvent *);

    private:
        QImage image;

};

