
#include <QApplication>
#include <stdio.h>
#include <unistd.h>

#include "mainwidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

	MainWidget mainWindow;
	mainWindow.show();

    return a.exec();
}
