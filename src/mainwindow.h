#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>

#include "tpglwindow.h"

namespace Ui {
class MainWindow;
}

/**
 * @brief Main window of the application displaying the OpenGL scene and user menus.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    /**
     * @brief Open a JSON particle system file in the editor and the scene
     * @param filename filename of the particle system configuration
     */
    void openFile(const QString& filename);
    /**
     * @brief Open a file dialog asking for a JSON config file
     */
    void openJsonFile();
    /**
     * @brief Open a JSON file from an entry in the right
     * @param index
     */
    void openTreeFile(const QModelIndex & index);

    /**
     * @brief Send the configuration written in the editor to the scene/tpglwindow
     */
    void apply();

    /**
     * @brief Open a file dialog and creates a new JSON file
     */
    void createFile();

    /**
     * @brief Saves the open json file
     */
    void save();

    /**
     * @brief Set the mouse behaviour in the scene. The user can either : move the emitter, move the camera or move the attract point.
     * @param index selector index
     */
    void changeMouseBehaviour(int index);

signals:
    void jsonChanged(const QString&);

private:
    Ui::MainWindow *ui;
    TPGLWindow * _glWindow;
    QWidget * _glWidgetContainer;

    QString _currentFile;
};

#endif // MAINWINDOW_H
