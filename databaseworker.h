#ifndef DATABASEWORKER_H
#define DATABASEWORKER_H
#include <QtSql>
#include <QString>
#include <QFile>
#include <QMessageBox>

class DatabaseWorker : public QObject
{
    Q_OBJECT
public:
    DatabaseWorker(QString path);
    QSqlDatabase messagesDatabase;

public slots:
    void DeleteSessionTable();
};

#endif // DATABASEWORKER_H
