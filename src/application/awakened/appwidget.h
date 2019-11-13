
#ifndef APPWIDGET_H
#define APPWIDGET_H

#include <QWidget>
#include <QObject>
#include <QListWidget>

namespace eink {

    class AppWidget : public QWidget
    {
        Q_OBJECT
        public:
            AppWidget(QWidget *parent = 0);
            ~AppWidget();

        private:
            void setupUI();
            void addAppIcon(const QString &);

        signals:
            void itemDoubleClicked(QListWidgetItem *);

        private:
            QListWidget *listWidget;
            QString appConfigFilePath;

    };

}

#endif
