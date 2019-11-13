
#include "openglplaywidget.h"
#include <stdlib.h>
#include <GL/glut.h>
#include <GL/freeglut.h>

namespace eink {

    class OPenglPlayWidgetPrivate
    {
    public:
        OPenglPlayWidgetPrivate()
        {
            argc = 1;
            argv[0] = "AwakenedWorld";
        }

        int argc = 1;
        char *argv[1];

    };

    OPenglPlayWidget::OPenglPlayWidget()
        : d(new OPenglPlayWidgetPrivate)
    {
        glutInit(&d->argc, d->argv);
    }

    OPenglPlayWidget::~OPenglPlayWidget()
    {
        if(d){
            delete d;
            d = NULL;
        }
    }

    void OPenglPlayWidget::initWindow(int width,int height)
    {
        printf("OPenglPlayWidget::initWindow Version: %s\n", glGetString(GL_VERSION));
        glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
        glutInitWindowPosition(0, 0);
        glutInitWindowSize(width, height);
        glutCreateWindow("OpenGL 3D View");

        glClearColor(0.0, 0.0, 0.0, 0.0);
        glMatrixMode(GL_PROJECTION);
        glOrtho(-5, 5, -5, 5, 5, 15);
        glMatrixMode(GL_MODELVIEW);
        gluLookAt(0, 0, 10, 0, 0, 0, 0, 1, 0);
    }

    void OPenglPlayWidget::startPlay()
    {
        glutDisplayFunc(vedioDisplay);
        glutMainLoopEvent();
    }

    void OPenglPlayWidget::vedioDisplay()
    {
        //在每帧渲染前，用指定的颜色清除color buffer ，清除颜色为glClearColor指定的颜色
        glClear(GL_COLOR_BUFFER_BIT);

        //交换前后缓冲，把渲染内容在屏幕上显示出来
        glutSwapBuffers();
    }

    void OPenglPlayWidget::createShader()
    {
    }

    void OPenglPlayWidget::createVertextShader()
    {
    }

    void OPenglPlayWidget::createFragmentShader()
    {
    }




    void OPenglPlayWidget::testUI()
    {
        glutInit(&d->argc, d->argv);
        glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
        glutInitWindowPosition(0, 0);
        glutInitWindowSize(300, 300);
        glutCreateWindow("OpenGL 3D View");

        glClearColor(0.0, 0.0, 0.0, 0.0);
        glMatrixMode(GL_PROJECTION);
        glOrtho(-5, 5, -5, 5, 5, 15);
        glMatrixMode(GL_MODELVIEW);
        gluLookAt(0, 0, 10, 0, 0, 0, 0, 1, 0);

        glutDisplayFunc(testDisplay);
        glutMainLoopEvent();
        //glutMainLoop();
    }

    void OPenglPlayWidget::testDisplay(void)
    {
       glClear(GL_COLOR_BUFFER_BIT);
       glColor3f(1.0, 0, 0);
       glutWireTeapot(3);
       glFlush();
    }

}
