
#include <QWidget>
#include <QObject>

#include "baseapp.h"

namespace eink{
    class ShowPictureWidget; //前向申明
}


class ImageProcessorWidget : public QWidget,public BaseApp
{
    Q_OBJECT
    public:
        ImageProcessorWidget(QWidget *parent = 0);
        ~ImageProcessorWidget();

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
        eink::ShowPictureWidget *showPictureWidget;

};
