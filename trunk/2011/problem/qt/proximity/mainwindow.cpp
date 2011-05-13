#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "directorymanager.h"
#include "settingsmanager.h"
#include "directorywatcher.h"
#include "hex.h"
#include "matchdialog.h"
#include "gamehandler.h"
#include "playercolor.h"
#include "competitiondialog.h"
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
    connect(GameHandler::Instance()->getMapHandler(), SIGNAL(Loaded()), this, SLOT(ChangeMap()));
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

void MainWindow::ChangeMap()
{
    QSqlQuery query;
    query.prepare("select name from competitions where id = :competition_id");
    query.bindValue(":competition_id", GameHandler::Instance()->CompetitionId());
    query.exec();
    query.first();
    QString compname = query.value(0).toString();
    query.prepare("select map_name from matches where id = :match_id");
    query.bindValue(":match_id", GameHandler::Instance()->GetMatchId());
    query.exec();
    query.first();
    ui->lMap->setText(QString("%1:%2 %3").arg(GameHandler::Instance()->MatchId()).arg(compname).arg(query.value(0).toString()));
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

void MainWindow::on_pbCompetition_clicked()
{
    CompetitionDialog *d = new CompetitionDialog(this);
    if (d->exec())
    {
        //adding competition
        //simple but yet effective
        QSqlQuery query;
        QVector<int> players;
        query.prepare("insert into competitions (name) values (:name)");
        query.bindValue(":name", d->Name());
        query.exec();
        query.exec("select last_insert_rowid()");
        query.first();
        int id = query.value(0).toInt();
        SettingsManager::Instance()->SetValue(GAME, COMPETITION_ID, id);
        for (int i = 0; i < d->PlayersCount(); i++)
        {
            query.prepare("insert into competition_players (competition_id, player_name, player) values (:competition_id, :player_name, :player)");
            query.bindValue(":competition_id", id);
            query.bindValue(":player_name", d->Player(i));
            query.bindValue(":player", bool(d->Player(i)=="<Player>"));
            query.exec();
            query.exec("select last_insert_rowid()");
            query.first();
            players.push_back(query.value(0).toInt());
        }
        int first_match = 0;
        for (int i = 0; i < d->PlayersCount(); i++)
        {
            for (int j = i + 1; j < d->PlayersCount(); j++)
            {
                for (int k = 0; k < d->MapsCount(); k++)
                {
                    query.prepare("insert into matches (competition_id, map_name) values (:competition_id, :map_name)");
                    query.bindValue(":competition_id", id);
                    query.bindValue(":map_name", d->Map(k));
                    query.exec();
                    query.exec("select last_insert_rowid()");
                    query.first();
                    int match = query.value(0).toInt();
                    if (!first_match)
                        first_match = match;
                    query.prepare("insert into player_competition_matches (competition_players_id, match_id) values (:competition_players_id, :match_id)");
                    query.bindValue(":competition_players_id", players[i]);
                    query.bindValue(":match_id", match);
                    query.exec();
                    query.prepare("insert into player_competition_matches (competition_players_id, match_id) values (:competition_players_id, :match_id)");
                    query.bindValue(":competition_players_id", players[j]);
                    query.bindValue(":match_id", match);
                    query.exec();

                    query.prepare("insert into matches (competition_id, map_name) values (:competition_id, :map_name)");
                    query.bindValue(":competition_id", id);
                    query.bindValue(":map_name", d->Map(k));
                    query.exec();
                    query.exec("select last_insert_rowid()");
                    query.first();
                    match = query.value(0).toInt();
                    if (!first_match)
                        first_match = match;
                    query.prepare("insert into player_competition_matches (competition_players_id, match_id) values (:competition_players_id, :match_id)");
                    query.bindValue(":competition_players_id", players[j]);
                    query.bindValue(":match_id", match);
                    query.exec();
                    query.prepare("insert into player_competition_matches (competition_players_id, match_id) values (:competition_players_id, :match_id)");
                    query.bindValue(":competition_players_id", players[i]);
                    query.bindValue(":match_id", match);
                    query.exec();
                }
            }
        }
        GameHandler::Instance()->Init();
    }

}

void MainWindow::on_pbFwd_clicked()
{
    //next match
    GameHandler::Instance()->NextMatch();
}

void MainWindow::on_pbBkwd_clicked()
{
    GameHandler::Instance()->PrevMatch();
}
