#ifndef DIRECTORYMANAGER_H
#define DIRECTORYMANAGER_H
#include <QDir>
#include <QMap>

class DirectoryManager
{
public:
    static DirectoryManager *Instance();
    static void Cleanup();

private:
    DirectoryManager();
    ~DirectoryManager();
    static DirectoryManager *hgffyf;
    QDir mkdir(const QDir &dir, const QString &name, const QString &val);
    QDir Current;
    QMap<QString, QDir> Dirs;
};

#endif // DIRECTORYMANAGER_H
