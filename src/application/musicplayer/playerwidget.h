
#include <QWidget>
#include <QObject>

class PlayerWidgetPrivate;

class PlayerWidget : public QWidget
{
    Q_OBJECT
    public:
        PlayerWidget(QWidget *parent = 0);
        ~PlayerWidget();

        void setFilePath(const QString &);
        void play();
        void stop();

        void currentMusicPlayFinished();
        void showPlayUI();
        void setCurPlayTimePos(int);

    private:
        void setupUI();
        void obtainFileInfotmation();
        void showFileInformation();

    private slots:
        void playStateClickedSlot(bool);
        void changePlayTimePosSlot(int );
        void signalMessageSlot(const QString &,const QString &);

    signals:
        void playNextMusicSig();
        void playLastMusicSig();

    private:
        PlayerWidgetPrivate *d;


};