#include <QApplication>
#include <QSurfaceFormat>
#include <QTime>

#include "mainwindow.h"

//====================================================================================================================================
int main(int argc, char **argv)
{
    QApplication app(argc, argv);


    qsrand(QTime::currentTime().msec());


    MainWindow window;
    window.resize(800, 600);
    window.show();

    return app.exec();
}
