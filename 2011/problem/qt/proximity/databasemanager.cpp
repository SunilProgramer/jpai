#include "databasemanager.h"
#include "QSqlField"

DatabaseManager *DatabaseManager::instance = 0;

DatabaseManager::DatabaseManager() : db(QSqlDatabase::addDatabase("QSQLITE"))
{
    db.setDatabaseName("proximity.db");
    if (!db.open())
    {
        qDebug("!");

//        QMessageBox::critical(0, qApp->tr("Cannot open database"),
//            qApp->tr("Unable to establish a database connection.\n"
//                     "This example needs SQLite support. Please read "
//                     "the Qt SQL driver documentation for information how "
//                     "to build it.\n\n"
//                     "Click Cancel to exit."), QMessageBox::Cancel);
//        return false;
    }
    q = QSqlQuery::QSqlQuery(db);
}

DatabaseManager::~DatabaseManager()
{

}

DatabaseManager *DatabaseManager::Instance()
{
    if (!instance)
        instance = new DatabaseManager;
    return instance;
}

void DatabaseManager::Cleanup()
{
    if (instance)
        delete instance;
}

bool DatabaseManager::exec(const QString &str)
{
    return q.exec(str);
}


