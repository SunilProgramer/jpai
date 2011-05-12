#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "directorymanager.h"
#include "settingsmanager.h"
#include "directorywatcher.h"
#include "hex.h"
#include "matchdialog.h"
#include "gamehandler.h"
#include "playercolor.h"
#include <QSqlQuery>
#include <QSqlError>

ScoresViewer::ScoresViewer(QObject *parent): QSqlQueryModel(parent)
{

}

QVariant ScoresViewer::data(const QModelIndex &index, int role) const
{
    QVariant value = QSqlQueryModel::data(index, role);
    if (value.isValid() && role == Qt::DisplayRole) {
            return value.toString();//.toUpper();
    }
    if (role == Qt::FontRole) {
        //if (index.column() == 0)
        //    return value.toString().prepend("#");
        //else if (index.column() == 2)
        //    return value.toString().toUpper();
        return QFont("Helvetica [Cronyx]", 12, 75);
    }
    if ((role == Qt::BackgroundRole || role == Qt::TextColorRole)&&index.column() == 0)
    {
        QModelIndex ind(this->index(index.row(), 0));

        return qVariantFromValue(PlayerColors::Color(GameHandler::Instance()->getMapHandler()->GetPlayerNum(this->data(ind, Qt::DisplayRole).toInt())));
    }
    return value;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    DirectoryManager::Instance();
    ui->setupUi(this);
    this->setWindowTitle(ProjectName);
    d = new Drawer(this);
    viewer = new ScoresViewer(this);
    MapDrawer *m = d->Add<MapDrawer>();
    m->SetMap(GameHandler::Instance()->getMapHandler());

    //DirectoryWatcher dd();
    //DatabaseDirectoryWatcher dd(this);
    //dd.AddWatcher(DIRECTORY_MAPS, (*DirectoryManager::Instance())(DIRECTORY_MAPS));
    connect(ui->pbMatch, SIGNAL(clicked()), this, SLOT(EditMatch()));
    connect(ui->pbStepFwd, SIGNAL(clicked()), GameHandler::Instance(), SLOT(StepFwd()));
    connect(ui->pbPlay, SIGNAL(clicked(bool)), GameHandler::Instance(), SLOT(Start(bool)));
    connect(ui->pbRestart, SIGNAL(clicked()), GameHandler::Instance()->getMapHandler(), SLOT(Reset()));
    connect(GameHandler::Instance()->getMapHandler(), SIGNAL(Loaded()), d, SLOT(ResetZoom()));
    //connect(GameHandler::Instance()->getMapHandler(), SIGNAL(Loaded()), this, SLOT(UpdateScores()));
    connect(GameHandler::Instance()->getMapHandler(), SIGNAL(Update()), this, SLOT(UpdateScores()));
    connect(GameHandler::Instance()->getAIHandler(), SIGNAL(Finished()), this, SLOT(Stop()));
    connect(GameHandler::Instance()->getAIHandler(), SIGNAL(StepFinished()), this, SLOT(UpdateScores()));

    d->show();
    ui->glDraw->addWidget(d);
}

void MainWindow::UpdateScores()
{
    if (!GameHandler::Instance()->GetMatchId())
        return;
    QSqlQuery query;
    query.prepare("select pcm.id, cp.player_name, scores from player_scores as ps inner join player_competition_matches as pcm on ps.player_competition_matches_id = pcm.id inner join matches on matches.id = pcm.match_id inner join competition_players as cp on cp.id = pcm.competition_players_id where matches.id = :match_id order by scores desc");
    query.bindValue(":match_id", GameHandler::Instance()->GetMatchId());
    query.exec();
    qDebug(query.lastError().text().toAscii());
    viewer->setQuery(query);
    viewer->setHeaderData(0, Qt::Horizontal, QObject::tr(""));
    viewer->setHeaderData(1, Qt::Horizontal, QObject::tr("Player"));
    viewer->setHeaderData(2, Qt::Horizontal, QObject::tr("Scores")); // to translate
    ui->tvScores->setModel(viewer);
    ui->tvScores->setColumnWidth(0, 24);
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
    UpdateScores();
}

void MainWindow::EditMatch()
{
    //check for running!
    MatchDialog *d = new MatchDialog(this, GameHandler::Instance()->CompetitionId(), GameHandler::Instance()->MatchId());
    //d->setWindowModality(Qt::WindowModal);
    if (d->exec())
        GameHandler::Instance()->ApplyMatch(d);
}
