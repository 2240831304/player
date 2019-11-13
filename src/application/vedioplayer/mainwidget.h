
#include <QWidget>
#include <QObject>

namespace eink{
    class PlayWidget; //前向申明
    class PlayManager;
}


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
        void doubleClickedsLOT(const QModelIndex &);
        void minimizeSlot();

    private:
        void quitApp();
        void resetEvn();

    private:
        eink::PlayManager *playerMgr;
        eink::PlayWidget *playerWidget;

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