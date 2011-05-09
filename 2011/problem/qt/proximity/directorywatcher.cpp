#include "directorywatcher.h"

DirectoryWatcher::DirectoryWatcher(const QString &directory): Directory(directory)
{
    addPath(Directory);
}
DirectoryWatcher::DirectoryWatcher(const QDir &directory): Directory(directory.absolutePath())
{
    addPath(Directory);
}
QString DirectoryWatcher::getDirectory()
{
    return Directory;
}

DatabaseDirectoryWatcher::~DatabaseDirectoryWatcher()
{
    for (int i = 0; i < Watchers.size(); i++)
        delete Watchers[i];
}

void DatabaseDirectoryWatcher::AddWatcher(const QString &Table, const QDir &directory)
{
    Tables.push_back(Table);
    Directories.push_back(directory);
    DirectoryWatcher *watcher = new DirectoryWatcher(directory);
    Watchers.push_back(watcher);
    connect(watcher, SIGNAL(fileChanged(QString)), this, SLOT(DirectoryChanged(QString)));
    connect(watcher, SIGNAL(directoryChanged(QString)), this, SLOT(DirectoryChanged(QString)));
    UpdateDatabase(Table, directory);
}

void DatabaseDirectoryWatcher::DirectoryChanged(const QString &directory)
{
    qDebug("1");
    for (int i = 0; i < Watchers.size(); i++)
        if (Directories[i] == directory)
            UpdateDatabase(Tables[i], Directories[i]);
}

void DatabaseDirectoryWatcher::UpdateDatabase(const QString &table, const QDir &directory)
{

}
