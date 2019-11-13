
#include <QWidget>
#include <QObject>

#include "baseapp.h"

namespace eink{
    class PlayWidget; //前向申明
}


class PlayerMainWidget : public QWidget,public BaseApp
{
    Q_OBJECT
    public:
        PlayerMainWidget(QWidget *parent=0);
        ~PlayerMainWidget();

        virtual void ExitApplication();

    private:
        void setupUI();

    protected:
		void paintEvent(QPaintEvent *);

    private slots:
        void doubleClickedsLOT(const QModelIndex &);

    private:
        void resetEvn();

    private:
        eink::PlayWidget *playerWidget;

};
