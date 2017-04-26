#ifndef MESSAGEWORKER_H
#define MESSAGEWORKER_H
#include <QObject>
#include <QDebug>
#include <qstring.h>

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
    void SyslogMessageParsed(QStringList messInfo,QString color);
};

#endif // MESSAGEWORKER_H
