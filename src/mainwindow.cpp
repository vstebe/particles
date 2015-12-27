#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QFileSystemModel>
#include <QTextStream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    _glWindow(new TPGLWindow)
{
    ui->setupUi(this);




    setCentralWidget(_glWindow);
    _glWindow->setFocusPolicy(Qt::StrongFocus);



    QFileSystemModel *model = new QFileSystemModel;
        ui->treeView->setModel(model);
        model->setNameFilters(QStringList("*.json"));
        model->setNameFilterDisables(false);
        ui->treeView->setRootIndex(model->setRootPath("../data"));
        for(int i=1; i<model->columnCount(); i++)
            ui->treeView->setColumnHidden(i, true);

    connect(ui->btnOpen, SIGNAL(clicked(bool)), this, SLOT(openJsonFile()));
    connect(ui->btnApply, SIGNAL(clicked(bool)), this, SLOT(apply()));
    connect(ui->btnNew, SIGNAL(clicked(bool)), this, SLOT(createFile()));
    connect(ui->btnSave, SIGNAL(clicked(bool)), this, SLOT(save()));

    connect(ui->treeView, SIGNAL(pressed(QModelIndex)), this, SLOT(openTreeFile(QModelIndex)));

    connect(ui->comboMouseBehaviour, SIGNAL(currentIndexChanged(int)), this, SLOT(changeMouseBehaviour(int)));

    connect(ui->sliderEmetters, SIGNAL(valueChanged(int)), _glWindow, SLOT(setNumberEmetters(int)));


    connect(this, SIGNAL(jsonChanged(QString)), _glWindow, SLOT(setJsonData(QString)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openFile(const QString &filename)
{
    _currentFile = filename;

    QFile file(filename);
    file.open(QIODevice::ReadOnly);

    QString json = file.readAll();

    ui->txtJson->setPlainText(json);

    emit jsonChanged(json);
}

void MainWindow::openJsonFile()
{
    QString filename = QFileDialog::getOpenFileName(this, "Open JSON File", NULL, "JSON File (*.json)");
    openFile(filename);
}

void MainWindow::openTreeFile(const QModelIndex &index)
{
    const QFileSystemModel * model = (const QFileSystemModel *) index.model();
    if(!model->fileInfo(index).isFile()) return;
    QString filename = model->fileInfo(index).absoluteFilePath();

    openFile(filename);
}

void MainWindow::apply()
{
    emit jsonChanged(ui->txtJson->toPlainText());
}

void MainWindow::createFile()
{
    QString filename = QFileDialog::getSaveFileName(this, "Open JSON File", NULL, "JSON File (*.json)");
    QFile file(filename);
    file.open(QIODevice::WriteOnly);
    file.close();

    openFile(filename);
}

void MainWindow::save()
{
    QFile file(_currentFile);
    file.open(QIODevice::WriteOnly);
    QTextStream outStream(&file);
    outStream << ui->txtJson->toPlainText();
    file.close();
}

void MainWindow::changeMouseBehaviour(int index)
{
    _glWindow->setMouseBehabiour((TPGLWindow::MouseBehaviour) index);
}
