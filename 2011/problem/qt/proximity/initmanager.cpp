#include "initmanager.h"

InitManager::InitManager()
{

}

InitManager::~InitManager()
{
    DatabaseManager::Cleanup();
    DirectoryManager::Cleanup();
    Hex::CleanUp();
    SettingsManager::Cleanup();
}

