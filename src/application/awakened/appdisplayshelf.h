
#ifndef APPDISPLAYSHELF_H
#define APPDISPLAYSHELF_H

#include <QWidget>
#include <QObject>

namespace eink {

    class AppDisplayShelfPrivate;
    class AppDisplayShelf : public QWidget
    {
        Q_OBJECT
        public:
            AppDisplayShelf(QWidget *parent = 0);
            ~AppDisplayShelf();

        public:
            void addApplication(const QString &,const QString &);
            void closeApps();

        private slots:
            void removeSubTabSlot(int);

        private:
            void setupUI();

        private:
            AppDisplayShelfPrivate *d;

    };



    class HomePageWidget : public QWidget
    {
        Q_OBJECT
        public:
            HomePageWidget(QWidget *parent = 0);
            ~HomePageWidget();

        protected:
			void paintEvent(QPaintEvent *);
    };

}

#endif