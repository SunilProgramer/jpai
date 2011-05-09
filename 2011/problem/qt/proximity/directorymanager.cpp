#include "directorymanager.h"
#include "definition.h"
#include "settingsmanager.h"

DirectoryManager *DirectoryManager::instance = 0;

DirectoryManager::DirectoryManager(): Current(QDir::current())
{
    //very bad
    QDir d;
    QString val;
    SettingsManager *sm = SettingsManager::Instance();
    sm->beginGroup("directories");

    val = sm->getValue(DIRECTORY_AIS, DIRECTORY_AIS).toString();
    mkdir(Current, DIRECTORY_AIS, val);

    val = sm->getValue(DIRECTORY_RUN, DIRECTORY_RUN).toString();
    mkdir(Current, DIRECTORY_RUN, val);

    val = sm->getValue(DIRECTORY_MAPS, DIRECTORY_MAPS).toString();
    mkdir(Current, DIRECTORY_MAPS, val);

    sm->endGroup();

}

QDir DirectoryManager::operator ()(const QString &name)
{
    return Dirs[name];
}

DirectoryManager::~DirectoryManager()
{
}

DirectoryManager *DirectoryManager::Instance()
{
    if (!instance)
        instance = new DirectoryManager();
    return instance;
}

void DirectoryManager::Cleanup()
{
    if (instance)
        delete instance; instance = 0;
}

QDir DirectoryManager::mkdir(const QDir &dir, const QString &name, const QString &val)
{
    QDir d = dir;
    if (!d.cd(val))
    {
        if (!d.mkdir(val) || !d.cd(val))
            throw ""; // <-- fix this
    }
    Dirs[name] = d;
    return d;
}
