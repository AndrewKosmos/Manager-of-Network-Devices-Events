#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QSettings>

class Settings : public QObject
{
    Q_OBJECT
public:
    Settings(QString configName);
    QSettings *settings;
    QString databasePath;
    QString defaultDBPath;

signals:

public slots:
    void SaveSettings(QString dbp);
    void LoadSettings();
};

#endif // SETTINGS_H
