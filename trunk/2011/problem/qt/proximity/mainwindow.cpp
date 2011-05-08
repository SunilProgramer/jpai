#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "directorymanager.h"
#include "settingsmanager.h"
#include "hex.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    DirectoryManager::Instance();
    ui->setupUi(this);
    this->setWindowTitle(ProjectName);
    d = new Drawer(this);
    map.Load("./maps/test.map");
    MapDrawer *m = d->Add<MapDrawer>();
    m->SetMap(map);

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
