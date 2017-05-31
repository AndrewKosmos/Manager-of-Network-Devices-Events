#include "mainwindow.h"
#include "networking.h"
#include "messageworker.h"
#include "databaseworker.h"
#include "customsqlmodel.h"
#include "settings.h"
#include <QApplication>
#include <QThread>
#include <QDir>
#include <QFile>
#include <QIODevice>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //Load Program Settings from .ini
    Settings *settings = new Settings("settings.ini");
    settings->LoadSettings();

    //Setup project stylesheet
    QFile projectStyle;
    projectStyle.setFileName(":/qss/qss/style.css");
    projectStyle.open(QFile::ReadOnly);
    QString qssStr = projectStyle.readAll();
    a.setStyleSheet(qssStr);

    //Setup messageworker and object for database working
    QThread *messageWorkerThread = new QThread;
    MessageWorker *messageWorker = new MessageWorker();
    messageWorker->moveToThread(messageWorkerThread);
    messageWorkerThread->start();

    QThread *databaseWorkerThread = new QThread;
    QString dbp = settings->databasePath;
    DatabaseWorker dbw(dbp);
    dbw.moveToThread(databaseWorkerThread);
    databaseWorkerThread->start();

    MainWindow w;
    w.show();

    /* Setup network tasks */
    QThread *SYSLOGManagerThread = new QThread;
    Networking *SYSLOGManager = new Networking(514);
    QObject::connect(SYSLOGManager,SIGNAL(DatagramReaded(QString,QString)),messageWorker,SLOT(ParseSyslogMess(QString,QString)));
    QObject::connect(messageWorker,SIGNAL(SyslogMessageParsed(QStringList)),&dbw,SLOT(InsertMessage(QStringList)),Qt::UniqueConnection);
    QObject::connect(&w,SIGNAL(ProgramIsClosing()),&dbw,SLOT(DeleteSessionValues()));
    QObject::connect(&dbw,SIGNAL(NewRowInserted()),&w,SLOT(RefreshTableView()));
    SYSLOGManager->moveToThread(SYSLOGManagerThread);
    SYSLOGManagerThread->start();

    Networking *SNMPManager = new Networking(161);
    return a.exec();
}
