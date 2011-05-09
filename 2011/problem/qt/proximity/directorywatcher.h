#ifndef DIRECTORYWATCHER_H
#define DIRECTORYWATCHER_H

#include <QFileSystemWatcher>
#include <QDir>
#include <QList>

class DirectoryWatcher : public QFileSystemWatcher
{
public:
    DirectoryWatcher(const QString &directory);
    DirectoryWatcher(const QDir &directory);
    QString getDirectory();
protected:
    QString Directory;
};

class DatabaseDirectoryWatcher : public QObject
{
    Q_OBJECT
public:
    //DatabaseDirectoryWatcher(QString s = "0"){}
    ~DatabaseDirectoryWatcher();
    QList<DirectoryWatcher*> Watchers;
    QList<QDir> Directories;
    QStringList Tables;
    void AddWatcher(const QString &Table, const QDir &directory);
public slots:
    void DirectoryChanged(const QString &directory);
protected:
    void UpdateDatabase(const QString &table, const QDir &directory);
};

#endif // DIRECTORYWATCHER_H
