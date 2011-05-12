#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "directorymanager.h"
#include "settingsmanager.h"
#include "directorywatcher.h"
#include "hex.h"
#include "matchdialog.h"
#include "gamehandler.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    DirectoryManager::Instance();
    ui->setupUi(this);
    this->setWindowTitle(ProjectName);
    d = new Drawer(this);
    MapDrawer *m = d->Add<MapDrawer>();
    m->SetMap(GameHandler::Instance()->getMapHandler());

    //DirectoryWatcher dd();
    //DatabaseDirectoryWatcher dd(this);
    //dd.AddWatcher(DIRECTORY_MAPS, (*DirectoryManager::Instance())(DIRECTORY_MAPS));
    connect(ui->pbMatch, SIGNAL(clicked()), this, SLOT(EditMatch()));
    connect(ui->pbStepFwd, SIGNAL(clicked()), GameHandler::Instance(), SLOT(StepFwd()));
    connect(ui->pbPlay, SIGNAL(clicked(bool)), GameHandler::Instance(), SLOT(Start(bool)));
    connect(GameHandler::Instance()->getMapHandler(), SIGNAL(Loaded()), d, SLOT(ResetZoom()));
    connect(GameHandler::Instance()->getAIHandler(), SIGNAL(Finished()), this, SLOT(Stop()));

    d->show();
    ui->glDraw->addWidget(d);
}

MainWindow::~MainWindow()
{
    delete d;
    delete ui;
}
void MainWindow::Stop()
{
    if (ui->pbPlay->isChecked())
        ui->pbPlay->toggle();
}
void MainWindow::showEvent(QShowEvent *event)
{
    GameHandler::Instance()->Init();
}

void MainWindow::EditMatch()
{
    //check for running!
    MatchDialog *d = new MatchDialog(this, GameHandler::Instance()->CompetitionId(), GameHandler::Instance()->MatchId());
    //d->setWindowModality(Qt::WindowModal);
    if (d->exec())
        GameHandler::Instance()->ApplyMatch(d);
}
