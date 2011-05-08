#include "databasemanager.h"

DatabaseManager *DatabaseManager::instance = 0;

DatabaseManager::DatabaseManager()
{
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

