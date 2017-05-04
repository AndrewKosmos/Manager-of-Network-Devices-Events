#ifndef MESSAGEWORKER_H
#define MESSAGEWORKER_H
#include <QObject>
#include <QDebug>
#include <qstring.h>
#include <QDateTime>
#include <QLocale>

class MessageWorker : public QObject
{
    Q_OBJECT
private slots:
    void ParseSyslogMess(QString message,QString IP);
public:
    MessageWorker();
    QStringList messageTypesList;
    QStringList messageColorsList;
signals:
    //void SyslogMessageParsed(QStringList messInfo,QString color);
    void SyslogMessageParsed(QStringList messInfo);
};

#endif // MESSAGEWORKER_H
