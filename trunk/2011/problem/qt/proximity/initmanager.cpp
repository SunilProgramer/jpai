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
        qDebug(e.text().toAscii());
    }
    query.exec("create table matches (id integer primary key autoincrement, "
                    "competition_id integer references competition(id), map_name text, started boolean default false)");
    query.exec("delete from matches where competition_id is NULL");
    query.exec("create table competition_players (id integer primary key autoincrement, "
                    "competition_id integer references competition(id), player_name text, player boolean default false)");
    if (!query.exec("delete from competition_players where competition_id is NULL;"))
    {
        QSqlError e = query.lastError();
        qDebug(e.text().toAscii());
    }
    QDir ai_dir = DirectoryManager::Instance()->Directory(DIRECTORY_AIS);
    QStringList files = ai_dir.entryList(QDir::Executable|QDir::Files, QDir::Name);
    while (!files.isEmpty())
    {
        query.prepare("insert into competition_players (player_name) VALUES (:filename)");
        query.bindValue(":filename", files.front());
        if (query.exec())// needs security
            files.pop_front();
        else
        {
            QSqlError e = query.lastError();
            qDebug(e.text().toAscii());
        }
    }
    query.exec("create table player_competition_matches (id integer primary key autoincrement, "
                    "competition_players_id integer references competition_players(id), match_id integer references matches(id))");
}

InitManager::~InitManager()
{
    DatabaseManager::Cleanup();
    DirectoryManager::Cleanup();
    Hex::CleanUp();
    SettingsManager::Cleanup();
    GameHandler::Cleanup();
}
