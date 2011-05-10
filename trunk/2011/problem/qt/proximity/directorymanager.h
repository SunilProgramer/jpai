#ifndef DIRECTORYMANAGER_H
#define DIRECTORYMANAGER_H
#include <QDir>
#include <QMap>

class DirectoryManager
{
public:
    static DirectoryManager *Instance();
    static void Cleanup();
    QDir Directory(const QString &name);

private:
    DirectoryManager();
    ~DirectoryManager();
    static DirectoryManager *instance;
    QDir mkdir(const QDir &dir, const QString &name, const QString &val);
    QDir Current;
    QMap<QString, QDir> Dirs;
};

#endif // DIRECTORYMANAGER_H
