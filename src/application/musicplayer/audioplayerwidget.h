
#include <QWidget>
#include <QObject>

#include "baseapp.h"

class PlayerWidget;
class FileManagerView;


class AudioPlayerWidget : public QWidget,public BaseApp
{
    Q_OBJECT
    public:
        AudioPlayerWidget(QWidget *parent=0);
        ~AudioPlayerWidget();

        virtual void ExitApplication();

    private:
        void setupUI();

    protected:
		void paintEvent(QPaintEvent *);

    private slots:
        void doubleClickedsLOT(const QModelIndex &);
        void systemMessage(const QString &, const QByteArray &);
        void playNextMusicSlot();
        void playLastMusicSlot();

    private:
        void quitApp();
        void resetEvn();

    private:
        PlayerWidget *playWidget;
        FileManagerView *fileView;
        bool isStartPlay;
        bool nextPlayFirstClicked;
        bool lastPlayFirstClicked;

};
