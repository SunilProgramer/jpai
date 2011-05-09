#ifndef DIRECTORYWATCHER_H
#define DIRECTORYWATCHER_H

#include <QFileSystemWatcher>
#include <QDir>
#include <QList>

class DirectoryWatcher : public QFileSystemWatcher
{
public:
    DirectoryWatcher(const QString &directory, QObject *parent = 0);
    DirectoryWatcher(const QDir &directory, QObject *parent = 0);
    QString getDirectory();
protected:
    QString Directory;
};

class DatabaseDirectoryWatcher : public QObject
{
    Q_OBJECT
public:
    DatabaseDirectoryWatcher(QObject *parent = 0);
    ~DatabaseDirectoryWatcher();
    QList<QFileSystemWatcher*> Watchers;
    QList<QDir> Directories;
    QStringList Tables;
    void AddWatcher(const QString &Table, const QDir &directory);
public slots:
    void DirectoryChanged(const QString &directory);
protected:
    void UpdateDatabase(const QString &table, const QDir &directory);
};

#endif // DIRECTORYWATCHER_H
