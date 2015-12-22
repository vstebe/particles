#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "tpglwindow.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void openFile(const QString& filename);
    void openJsonFile();
    void openTreeFile(const QModelIndex & index);
    void apply();
    void createFile();
    void save();

signals:
    void jsonChanged(const QString&);

private:
    Ui::MainWindow *ui;
    TPGLWindow * _glWindow;

    QString _currentFile;
};

#endif // MAINWINDOW_H
