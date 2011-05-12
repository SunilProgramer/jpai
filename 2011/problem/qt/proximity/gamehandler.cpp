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
    int cid = CompetitionId();
    QSqlQuery q;
    q.setForwardOnly(true);
    QString s = "select * from matches where competition_id ";
    if (cid)
        s += "= :competition_id";
    else
        s += " is NULL";
    q.bindValue(":competition_id", cid);
    q.prepare(s);
    q.exec();
    if (!q.first())
    {
        SettingsManager::Instance()->SetValue(GAME, MATCH_ID, int(0));
        match_id = 0;
    }
    else
    {
        match_id = q.value(0).toInt();
        q.prepare("select * from player_competition_matches inner join competition_players on"
                  " competition_id = competition_players.competition_id where match_id = :match_id");
        q.bindValue(":match_id", match_id);
    }
}
GameHandler::~GameHandler()
{
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
    Handler.Start();
    Handler.Stop();
}

void GameHandler::Start(bool start)
{
    if (start)
        Handler.Start();
    else
        Handler.Stop();
}

int GameHandler::CompetitionId()
{
    return SettingsManager::Instance()->getValue(GAME, COMPETITION_ID, int(0)).toInt();
}

MapHandler *GameHandler::getMapHandler()
{
    return &m;
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
            qDebug(e.text().toAscii());
        }
        m.Load(DirectoryManager::Instance()->Directory(DIRECTORY_MAPS).filePath(s));
        static QRegExp r("^(\\d+):(\\w+\\.?\\w+)\\s(\\w+)");//");
        m.ClearPlayers();
        for (int i = 0; i < dialog->lwAis->count(); i++)
        {
            QString s = dialog->lwAis->item(i)->text();
            if (!r.exactMatch(s))
                qDebug("!");
            QStringList l = r.capturedTexts();
            m.AddPlayer(l[2], l[1].toInt(), l[3] != "Comp");
        }
    }
    else
    {
        //modify settings and players
    }

}
