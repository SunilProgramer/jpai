#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

class DatabaseManager
{
public:
    static DatabaseManager *Instance();
    static void Cleanup();

private:
    DatabaseManager();
    ~DatabaseManager();
    static DatabaseManager *instance;
};

#endif // DATABASEMANAGER_H
