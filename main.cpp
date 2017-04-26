#include "mainwindow.h"
#include "networking.h"
#include "messageworker.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    MessageWorker *messageWorker = new MessageWorker();

    /* Setup network tasks */
    Networking *SYSLOGManager = new Networking(514);
    QObject::connect(SYSLOGManager,SIGNAL(DatagramReaded(QString,QString)),messageWorker,SLOT(ParseSyslogMess(QString,QString)));
    QObject::connect(messageWorker,SIGNAL(SyslogMessageParsed(QStringList,QString)),&w,SLOT(GetParsedSyslogMess(QStringList,QString)));
    return a.exec();
}
