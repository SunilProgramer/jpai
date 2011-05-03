#include "settingsmanager.h"
#include "definition.h"

SettingsManager *SettingsManager::inst = 0;

SettingsManager *SettingsManager::Instance()
{
    if (!SettingsManager::inst)
        SettingsManager::inst = new SettingsManager();
    return SettingsManager::inst;
}

void SettingsManager::Cleanup()
{
    if (SettingsManager::inst)
        delete SettingsManager::inst; SettingsManager::inst = 0;
}

QVariant SettingsManager::getValue(const QString &key, const QVariant &defaultValue)
{
    QVariant val;
    setValue(key, val = value(key, defaultValue));
    return val;
}

SettingsManager::SettingsManager(): QSettings(SectionName, ProjectName)
{
}
