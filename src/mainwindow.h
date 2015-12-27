#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>

#include "tpglwindow.h"

class BigFocusWidget : public QWidget {
public:
    BigFocusWidget(QWidget * widget = NULL) :  QWidget(widget) {setFocusPolicy(Qt::StrongFocus);}
    void mousePressEvent(QMouseEvent *) {
        qDebug() << "tet";
        setFocus();
    }
};

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
    void changeMouseBehaviour(int index);

signals:
    void jsonChanged(const QString&);

private:
    Ui::MainWindow *ui;
    TPGLWindow * _glWindow;
    QWidget * _glWidgetContainer;
    BigFocusWidget * _glWidgetContainerContainer;

    QString _currentFile;
};

#endif // MAINWINDOW_H
