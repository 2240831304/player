
#ifndef APPTITLEWIDGET_H
#define APPTITLEWIDGET_H

#include <QWidget>
#include <QObject>

class QPushButton;
class QLabel;

namespace eink{

    class AppTitleWidget : public QWidget
    {
        Q_OBJECT
        public:
           AppTitleWidget(QWidget *parent = 0);
           ~AppTitleWidget();

        public:
            void setTitleName(const QString &);
            void setCloseButShow(bool);

        private:
            void setupUI();

        private:
            QPushButton *pushButton;
            QLabel *titleNameLab;
    };

}

#endif