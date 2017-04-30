#include "mainwindow.h"
#include "networking.h"
#include "messageworker.h"
#include "databaseworker.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    MessageWorker *messageWorker = new MessageWorker();
    DatabaseWorker dbw("D:/test.db");

    /* Setup network tasks */
    Networking *SYSLOGManager = new Networking(514);
    QObject::connect(SYSLOGManager,SIGNAL(DatagramReaded(QString,QString)),messageWorker,SLOT(ParseSyslogMess(QString,QString)));
    QObject::connect(messageWorker,SIGNAL(SyslogMessageParsed(QStringList,QString)),&w,SLOT(GetParsedSyslogMess(QStringList,QString)));
    QObject::connect(&w,SIGNAL(ProgramIsClosing()),&dbw,SLOT(DeleteSessionTable()));
    return a.exec();
}
