#include "settings.h"
#include <QDebug>
#include <QDir>

Settings::Settings(QString configName)
{
    settings = new QSettings(configName,QSettings::IniFormat);
}

void Settings::LoadSettings()
{
    defaultDBPath = QString(QDir::homePath() + "/test.db");
    databasePath = settings->value("DatabaseSettings/databasePath",QString(QDir::homePath() + "/test.db")).toString();
}

void Settings::SaveSettings(QString dbp)
{
    settings->setValue("DatabaseSettings/databasePath",dbp);
    settings->sync();
}
