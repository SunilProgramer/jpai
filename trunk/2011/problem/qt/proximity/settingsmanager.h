#ifndef SETTINGSMANAGER_H
#define SETTINGSMANAGER_H

#include <QSettings>

class SettingsManager : public QSettings
{
public:
    static SettingsManager *Instance();
    static void Cleanup();
    QVariant getValue(const QString &key, const QVariant &defaultValue);
    QVariant getValue(const QString &group, const QString &key, const QVariant &defaultValue);
private:
    SettingsManager();
    static SettingsManager *instance;
};


#endif // SETTINGSMANAGER_H
