#include "mainwindow.h"
#include "networking.h"
#include "messageworker.h"
#include "databaseworker.h"
#include "customsqlmodel.h"
#include <QApplication>
#include <QDir>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //Setup messageworker and object for database working
    MessageWorker *messageWorker = new MessageWorker();
    DatabaseWorker dbw(QDir::homePath() + "/test.db");

    MainWindow w;
    w.show();

    /* Setup network tasks */
    Networking *SYSLOGManager = new Networking(514);
    QObject::connect(SYSLOGManager,SIGNAL(DatagramReaded(QString,QString)),messageWorker,SLOT(ParseSyslogMess(QString,QString)));
    QObject::connect(messageWorker,SIGNAL(SyslogMessageParsed(QStringList)),&dbw,SLOT(InsertMessage(QStringList)),Qt::UniqueConnection);
    QObject::connect(&w,SIGNAL(ProgramIsClosing()),&dbw,SLOT(DeleteSessionValues()));
    QObject::connect(&dbw,SIGNAL(NewRowInserted()),&w,SLOT(RefreshTableView()));
    return a.exec();
}
