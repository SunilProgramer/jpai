#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "directorymanager.h"
#include "settingsmanager.h"
#include "map.h"
#include "hex.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    DirectoryManager::Instance();
    ui->setupUi(this);
    this->setWindowTitle(ProjectName);
    d = new Drawer(this);
    Map *m = new Map(true);
    d->Add(m);
    d->show();
    ui->glDraw->addWidget(d);
}

MainWindow::~MainWindow()
{
    DirectoryManager::Cleanup();
    SettingsManager::Cleanup();
    Hex::CleanUp();
    delete d;
    delete ui;
}
