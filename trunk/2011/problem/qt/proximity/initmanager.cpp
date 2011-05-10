#include "initmanager.h"
#include "definition.h"
#include <QSqlError>
#include <QSqlDriver>

InitManager::InitManager()
{
    DatabaseManager *db = DatabaseManager::Instance();// -?
    QSqlQuery query(QSqlDatabase::database());
    query.exec("create table competitions (id integer primary key autoincrement, "
                    "name text) if not exists");
    query.exec("create table matches (id integer primary key autoincrement, "
                    "competition_id integer references competition(id), map_name text) if not exists");
    query.exec("create table competition_players (id integer primary key autoincrement, "
                    "competition_id integer references competition(id), player_name text) if not exists");
    query.exec("delete from competition_players where competition_id = NULL");
    QDir ai_dir = DirectoryManager::Instance()->Directory(DIRECTORY_AIS);
    QStringList files = ai_dir.entryList(QDir::Executable|QDir::Files, QDir::Name);
    while (!files.isEmpty())
    {
        if (query.exec("insert into competition_players (player_name) VALUES (\"" + db->EscapeString(files.front()) + "\")"))// need security
            files.pop_front();
        else
        {
            QSqlError e = query.lastError();
            qDebug(e.text().toAscii());
        }
    }
    query.exec("create table player_competition_matches (id integer primary key autoincrement, "
                    "competition_players_id integer references competition_players(id), player text)");
}

InitManager::~InitManager()
{
    DatabaseManager::Cleanup();
    DirectoryManager::Cleanup();
    Hex::CleanUp();
    SettingsManager::Cleanup();
}

