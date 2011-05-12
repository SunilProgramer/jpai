#include "directorywatcher.h"

DirectoryWatcher::DirectoryWatcher(const QString &directory, QObject *parent): QFileSystemWatcher(parent), Directory(directory)
{
    addPath(Directory + "/map20x3.map");
}
DirectoryWatcher::DirectoryWatcher(const QDir &directory, QObject *parent): QFileSystemWatcher(parent), Directory(directory.absolutePath())
{
    addPath(Directory + "/map20x3.map");
}
QString DirectoryWatcher::getDirectory()
{
    return Directory;
}

DatabaseDirectoryWatcher::DatabaseDirectoryWatcher(QObject *parent) : QObject(parent)
{

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
    QStringList l;
    l << (directory.absolutePath() + "/");
    QFileSystemWatcher *watcher = new QFileSystemWatcher(l);
    //watcher->addPath(directory);
    Watchers.push_back(watcher);
    l = watcher->files();
    l = watcher->directories();
    connect(watcher, SIGNAL(fileChanged(QString)), this, SLOT(DirectoryChanged(QString)));
    connect(watcher, SIGNAL(directoryChanged(QString)), this, SLOT(DirectoryChanged(QString)));
    UpdateDatabase(Table, directory);
}

void DatabaseDirectoryWatcher::DirectoryChanged(const QString &directory)
{
    //qDebug("1");
    for (int i = 0; i < Watchers.size(); i++)
        if (Directories[i] == directory)
            UpdateDatabase(Tables[i], Directories[i]);
}

void DatabaseDirectoryWatcher::UpdateDatabase(const QString &table, const QDir &directory)
{

}
