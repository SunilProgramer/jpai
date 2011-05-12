#include "settingsmanager.h"
#include "definition.h"

SettingsManager *SettingsManager::instance = 0;

SettingsManager *SettingsManager::Instance()
{
    if (!instance)
        instance = new SettingsManager();
    return instance;
}

void SettingsManager::Cleanup()
{
    if (instance)
        delete instance; instance = 0;
}

QVariant SettingsManager::getValue(const QString &key, const QVariant &defaultValue)
{
    QVariant val;
    setValue(key, val = value(key, defaultValue));
    return val;
}

QVariant SettingsManager::getValue(const QString &group, const QString &key, const QVariant &defaultValue)
{
    QVariant val;
    beginGroup(group);
    setValue(key, val = value(key, defaultValue));
    endGroup();
    return val;
}
void SettingsManager::SetValue(const QString &group, const QString &key, const QVariant &Value)
{
    beginGroup(group);
    setValue(key, Value);
    endGroup();
}

SettingsManager::SettingsManager(): QSettings(SectionName, ProjectName)
{
}
