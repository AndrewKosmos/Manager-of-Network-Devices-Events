#include "mainwindow.h"
#include "networking.h"
#include "messageworker.h"
#include "databaseworker.h"
#include "customsqlmodel.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //Setup messageworker and object for database working
    MessageWorker *messageWorker = new MessageWorker();
    DatabaseWorker dbw("D:/test.db");

    MainWindow w;
    //w.model->setQuery("select Messages_Table.Datetime, Facility_Table.IP,Facility_Table.Name,Messages_Table.Type,Messages_Table.Tag,Messages_Table.Message from Messages_Table,Facility_Table Where Messages_Table.Facility_ID = Facility_Table.Facility_ID");
    w.show();

    /* Setup network tasks */
    Networking *SYSLOGManager = new Networking(514);
    QObject::connect(SYSLOGManager,SIGNAL(DatagramReaded(QString,QString)),messageWorker,SLOT(ParseSyslogMess(QString,QString)));
    QObject::connect(messageWorker,SIGNAL(SyslogMessageParsed(QStringList)),&dbw,SLOT(InsertMessage(QStringList)),Qt::UniqueConnection);
    QObject::connect(&w,SIGNAL(ProgramIsClosing()),&dbw,SLOT(DeleteSessionValues()));
    QObject::connect(&dbw,SIGNAL(NewRowInserted()),&w,SLOT(RefreshTableView()));
    return a.exec();
}
