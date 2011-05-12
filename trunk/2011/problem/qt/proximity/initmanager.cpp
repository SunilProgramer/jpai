#include "initmanager.h"
#include "definition.h"
#include <QSqlError>
#include <QSqlDriver>

InitManager::InitManager()
{
    DatabaseManager *db = DatabaseManager::Instance();// -?
    QSqlQuery query(QSqlDatabase::database());
    if (!query.exec("create table competitions (id integer primary key autoincrement, "
                    "name text)"))
    {
        QSqlError e = query.lastError();
        //qDebug(e.text().toAscii());
    }
    query.exec("create table matches (id integer primary key autoincrement, "
                    "competition_id integer references competition(id), map_name text, started boolean default false, finished boolean default false)");
    //query.exec("delete from matches where competition_id is NULL");
    query.exec("create table competition_players (id integer primary key autoincrement, "
                    "competition_id integer references competition(id), player_name text, player boolean default false)");
    QDir ai_dir = DirectoryManager::Instance()->Directory(DIRECTORY_AIS);
    QStringList files = ai_dir.entryList(QDir::Executable|QDir::Files, QDir::Name);
    while (!files.isEmpty())
    {
        query.prepare("select * from competition_players where player_name = :filename");
        query.bindValue(":filename", files.front());
        if (query.exec() && !query.first())
        {
            //query.prepare("delete from competition_players where id = :id");
            //query.bindValue(":id", query.value(0).toInt());
            query.prepare("insert into competition_players (player_name) VALUES (:filename)");
            query.bindValue(":filename", files.front());
            query.exec();// needs security
        }
        files.pop_front();
    }
    query.exec("create table player_competition_matches (id integer primary key autoincrement, "
                    "competition_players_id integer references competition_players(id), match_id integer references matches(id))");
    query.exec("create table player_scores (id integer primary key autoincrement, "
                    "player_competition_matches_id integer references player_competition_matches(id), scores integer default 0)");
    query.exec("create table player_moves (id integer primary key autoincrement, "
                    "player_competition_matches_id integer references player_competition_matches(id), step integer default 0,"
                    "player integer default 1, influence integer default 0, current_step integer default 0,"
                    "x integer default 0, y integer default 0)");
    qDebug(query.lastError().text().toAscii());
}

InitManager::~InitManager()
{
    DatabaseManager::Cleanup();
    DirectoryManager::Cleanup();
    Hex::CleanUp();
    SettingsManager::Cleanup();
    GameHandler::Cleanup();
}

