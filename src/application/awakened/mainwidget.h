
#include <QWidget>
#include <QObject>
#include <QListWidgetItem>

class MainWidgetPrivate;
class MainWidget : public QWidget
{
    Q_OBJECT
    public:
        MainWidget(QWidget *parent=0);
        ~MainWidget();

    private:
        void setupUI();

    protected:
		void paintEvent(QPaintEvent *);


    private slots:
        void closeSlot();
        void minimizeSlot();
        void itemDoubleClickedSlot(QListWidgetItem *);

    private:
        void quitApp();
        void resetEvn();

    private:
        MainWidgetPrivate *d;
};


class TitleBar : public QWidget
{
    Q_OBJECT
    public:
        TitleBar(QWidget *parent = 0);
        ~TitleBar();

    private:
        void setupUI();

    protected:
        void paintEvent(QPaintEvent *);
        void mousePressEvent (QMouseEvent *event);
        void mouseReleaseEvent(QMouseEvent *event);
        void mouseMoveEvent ( QMouseEvent * event);

    signals:
        void close();
        void minimize();

    private:
        QPoint  formerMainPos;  //移动前窗口左上角的位置
        QPoint pressedPos;
        bool  mousePress;

};