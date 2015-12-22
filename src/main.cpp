#include <QApplication>
#include <QSurfaceFormat>
#include <QTime>

#include "mainwindow.h"

//====================================================================================================================================
int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    QSurfaceFormat format;

    qsrand(QTime::currentTime().msec());

#ifdef Q_OS_LINUX  // what about Linux ?
    format.setMajorVersion(3);
    format.setMinorVersion(3);
    format.setProfile(QSurfaceFormat::CoreProfile);
#endif
    format.setSamples(16);
    format.setDepthBufferSize(24);

    MainWindow window;
    //window.setFormat(format);
    window.resize(800, 600);
    window.show();

    return app.exec();
}
