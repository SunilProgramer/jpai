#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "drawer.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle(ProjectName);
    Drawer *d = new Drawer(this);
    d->show();
    ui->glDraw->addWidget(d);
}

MainWindow::~MainWindow()
{
    delete ui;
}
