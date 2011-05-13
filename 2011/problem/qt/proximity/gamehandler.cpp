#include "gamehandler.h"
#include "settingsmanager.h"
#include "directorymanager.h"
#include "definition.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QRegExp>

GameHandler *GameHandler::instance = 0;

GameHandler::GameHandler()
{
    Handler.SetMapHandler(&m);
}
GameHandler::~GameHandler()
{
}
void GameHandler::Init()
{
    int cid = CompetitionId();
    QSqlQuery q;
    q.setForwardOnly(true);
    QString s = "select * from matches where competition_id ";
    if (cid)
        s += "= :competition_id";
    else
        s += " is NULL";
    q.prepare(s);
    q.bindValue(":competition_id", cid);
    q.exec();
    q.first();
    qDebug(q.lastError().text().toAscii());
    if (!q.isValid())
    {
        SettingsManager::Instance()->SetValue(GAME, MATCH_ID, int(0)); // next task
        match_id = 0;
    }
    else
    {
        match_id = q.value(0).toInt();
        LoadMatchFromDb();
    }
}

void GameHandler::LoadMatchFromDb()
{
    QSqlQuery query;
    query.prepare("select * from matches where id = :id");
    query.bindValue(":id", match_id);
    query.exec();
    query.first();
    //qDebug(query.lastError().text().toAscii());
    m.Load(DirectoryManager::Instance()->Directory(DIRECTORY_MAPS).filePath(query.value(2).toString()));
    m.ClearPlayers();
    query.prepare("select pcm.id, cp.player_name, cp.player from competition_players as cp "
                  "inner join player_competition_matches as pcm on cp.id = pcm.competition_players_id where pcm.match_id = :match_id");
    query.bindValue(":match_id", match_id);
    query.exec();
    while (query.next())
    {
        m.AddPlayer(query.value(1).toString(), query.value(0).toInt(), query.value(2).toBool());
    }
}
bool GameHandler::NotStarted()
{
    QSqlQuery query;
    query.prepare("select count() from player_moves where step >= :step");
    query.bindValue(":step", m.Stepped());
    query.exec();
    query.first();
    return query.value(0).toInt();
}

void GameHandler::SilentStep()
{
    QSqlQuery query;
    query.prepare("select min(current_step) from player_moves as ps inner_join player_competition_matches as pcm where step > :step, pcm.match_id = :match_id");
    query.bindValue(":step", m.Stepped());
    query.bindValue(":match_id", match_id);
    query.exec();
    query.first();
    int cs = query.value(0).toInt();
    query.prepare("select pcm.id, x, y, player, influence from player_moves as ps"
                  "inner join player_competition_matches as pcm on pcm.id = player_competition_matches_id "
                  "where step >= :step and pcm.match_id = :match_id and current_step = :current_step");
    query.bindValue(":step", m.Stepped());
    query.bindValue(":current_step", cs);
    query.bindValue(":match_id", match_id);
    query.exec();
    while (query.next())
    {
        m.silentStep(query.value(1).toInt(), query.value(2).toInt(), query.value(3).toInt(), query.value(4).toInt());
    }
    m.SetCurrentStep(cs);
}

GameHandler *GameHandler::Instance()
{
    if (!instance)
        instance = new GameHandler;
    return instance;
}
void GameHandler::Cleanup()
{
    if (instance)
        delete instance; // ?!
    return;
}
void GameHandler::StepFwd()
{
    //protect match_id ?!
   // if (NotStarted())
    //{
    //    SilentStep();
    //}
    Handler.Start();
    Handler.Stop();
}

void GameHandler::NextMatch()
{
    QSqlQuery query;
    query.prepare("select id from matches where id > :match_id and competition_id = :competition_id");
    query.bindValue(":match_id", match_id);
    query.bindValue(":competition_id", CompetitionId());
    query.exec();
    query.first();
    if (query.isValid())
    {
        match_id = query.value(0).toInt();
        LoadMatchFromDb();
    }
}

void GameHandler::PrevMatch()
{
    QSqlQuery query;
    query.prepare("select id from matches where id < :match_id and competition_id = :competition_id order by id desc");
    query.bindValue(":match_id", match_id);
    query.bindValue(":competition_id", CompetitionId());
    query.exec();
    query.first();
    if (query.isValid())
    {
        match_id = query.value(0).toInt();
        LoadMatchFromDb();
    }
}

void GameHandler::Start(bool start)
{
    if (start)
        Handler.Start();
    else
        Handler.Stop();
}
int GameHandler::GetMatchId()
{
    return match_id;
}

int GameHandler::CompetitionId()
{
    return SettingsManager::Instance()->getValue(GAME, COMPETITION_ID, int(0)).toInt();
}
int GameHandler::MatchId()
{
    return match_id;
}

MapHandler *GameHandler::getMapHandler()
{
    return &m;
}
AIHandler *GameHandler::getAIHandler()
{
    return &Handler;
}

void GameHandler::ApplyMatch(MatchDialog *dialog)
{
    if (dialog->lwAis->count() < 2)
        return;
    int cid = CompetitionId();
    if (match_id == 0)
    {
        QSqlQuery q;
        QString s = dialog->cbMaps->itemData(dialog->cbMaps->currentIndex()).toString();
        if (!cid)
        {
            q.prepare("insert into matches (map_name) values ( :map_name )");
            q.bindValue(":map_name", s);
        }
        else
        {
            q.prepare("insert into matches (competition_id, map_name) values (:competition_id, :map_name)");
            q.bindValue(":competition_id", cid);
            q.bindValue(":map_name", s);
        }
        if (!q.exec())
        {
            QSqlError e = q.lastError();
            //qDebug(e.text().toAscii());
        }
        q.exec("select last_insert_rowid()");
        q.first();
        match_id = q.value(0).toInt();
        m.Load(DirectoryManager::Instance()->Directory(DIRECTORY_MAPS).filePath(s));
        m.ClearPlayers();
        static QRegExp r("^(\\d+):(\\w+\\.?\\w+)\\s(\\w+)");//");
        for (int i = 0; i < dialog->lwAis->count(); i++)
        {
            QString s = dialog->lwAis->item(i)->text();
            if (!r.exactMatch(s))
                qDebug("!");
            QStringList l = r.capturedTexts();
            q.prepare("insert into player_competition_matches (competition_players_id, match_id) values (:competition_players_id, :match_id)");
            q.bindValue(":competition_players_id", l[1].toInt());
            q.bindValue(":match_id", match_id);
            q.exec();
            q.exec("select last_insert_rowid()");
            q.first();
            m.AddPlayer(l[2], q.value(0).toInt(), l[3] != "Comp");
        }
    }
    else
    {
        QSqlQuery query;
        bool needs_reset = false;
        bool ai_changed = false;
        query.prepare("select map_name from matches where id = :id");
        query.bindValue(":id", match_id);
        query.exec();
        query.first();
        if (query.value(0).toString() != dialog->cbMaps->itemData(dialog->cbMaps->currentIndex()).toString())
        {
            query.prepare("update matches set map_name = :map_name where id = :match_id");
            query.bindValue(":map_name", dialog->cbMaps->itemData(dialog->cbMaps->currentIndex()).toString());
            query.bindValue(":match_id", match_id);
            query.exec();
            m.Load(DirectoryManager::Instance()->Directory(DIRECTORY_MAPS).filePath(dialog->cbMaps->itemData(dialog->cbMaps->currentIndex()).toString()));
            needs_reset = true;
        }
        static QRegExp r("^(\\d+):(\\w+\\.?\\w+)\\s(\\w+)");//");
        for (int i = 0; i < dialog->lwAis->count(); i++)
        {
            QString s = dialog->lwAis->item(i)->text();
            if (!r.exactMatch(s))
                qDebug("!");
            QStringList l = r.capturedTexts();
            if (l[1].toInt() != m.GetPlayerId(i))
            {
                ai_changed = true;
                break;
            }
        }
        if (dialog->lwAis->count() != m.PlayersCount() || ai_changed)
        {
            m.ClearPlayers();
            query.prepare("delete from player_competition_matches where match_id = :match_id");
            query.bindValue(":match_id", match_id);
            query.exec();
            for (int i = 0; i < dialog->lwAis->count(); i++)
            {
                QString s = dialog->lwAis->item(i)->text();
                if (!r.exactMatch(s))
                    qDebug("!");
                QStringList l = r.capturedTexts();
                query.prepare("insert into player_competition_matches (competition_players_id, match_id) values (:competition_players_id, :match_id)");
                query.bindValue(":competition_players_id", l[1].toInt());
                query.bindValue(":match_id", match_id);
                query.exec();
                query.exec("select last_insert_rowid()");
                query.first();
                m.AddPlayer(l[2], query.value(0).toInt(), l[3] != "Comp");
            }
            needs_reset = true;
        }
        if (needs_reset)
            m.Reset();
    }
}
