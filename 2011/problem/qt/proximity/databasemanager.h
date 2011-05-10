#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QSqlDatabase>
#include <QSqlQuery>

class DatabaseManager
{
public:
    static DatabaseManager *Instance();
    static void Cleanup();
    bool exec(const QString &str);
    QString EscapeString(const QString &val);
private:
    QSqlDatabase db;
    QSqlQuery q;
    DatabaseManager();
    ~DatabaseManager();
    static DatabaseManager *instance;
};

#endif // DATABASEMANAGER_H
