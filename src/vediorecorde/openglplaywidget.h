
#ifndef OPENGLPLAYWIDGET_H
#define OPENGLPLAYWIDGET_H

#include <QObject>
#include <QThread>

namespace eink {

class OPenglPlayWidgetPrivate;
class OPenglPlayWidget
{
    public:
        OPenglPlayWidget();
        ~OPenglPlayWidget();
        void testUI();

        void initWindow(int width,int height);
        void startPlay();
        void createShader();

    private:
        void createVertextShader();
        void createFragmentShader();

    private:
        static void testDisplay(void);
        static void vedioDisplay();


    private:
       OPenglPlayWidgetPrivate *d;
};

}

#endif
