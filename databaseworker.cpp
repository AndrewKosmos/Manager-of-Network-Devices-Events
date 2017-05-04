#include "databaseworker.h"

DatabaseWorker::DatabaseWorker(QString path)
{
    //initialize database and set path to the database file
    messagesDatabase = QSqlDatabase::addDatabase("QSQLITE");
    messagesDatabase.setDatabaseName(path);

    //if file exists - just open database
    //else - create and the open database

    if(QFile::exists(path))
    {
        messagesDatabase.setDatabaseName(path);
        messagesDatabase.open();
    }
    else
    {
        messagesDatabase.setDatabaseName(path);
        messagesDatabase.open();
        QSqlQuery create_facility_table("CREATE TABLE Facility_Table (Facility_ID integer NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,IP varchar(15) NOT NULL UNIQUE,Name text NOT NULL UNIQUE)");
        create_facility_table.exec();
        QSqlQuery create_messages_table("CREATE TABLE Messages_Table (Message_ID integer NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,Datetime varchar(255) NOT NULL,Facility_ID integer NOT NULL,Type text NOT NULL,Tag text NOT NULL,Message text NOT NULL, isNew integer NOT NULL, FOREIGN KEY (Facility_ID) REFERENCES Facility_Table (Facility_ID))");
        create_messages_table.exec();
    }
}

void DatabaseWorker::DeleteSessionValues()
{
    //set all fields isNew with true sign to false in table for messages.
    //isNew - special field for marking session messages
    QSqlQuery delete_session_values("UPDATE Messages_Table SET isNew = 0 WHERE isNEW = 1");
    delete_session_values.exec();
}

void DatabaseWorker::InsertMessage(QStringList messinfo)
{
    QString IP = messinfo.at(1);
    QSqlQuery getCountRows("select count(*) from Facility_Table where IP = '" + IP + "'");
    //int count = getCountRows.exec();
    getCountRows.exec();
    getCountRows.next();
    int count = getCountRows.value(0).toInt();

    if(count == 0)
    {
        QSqlQuery insertIntoFac("insert into Facility_table(IP,Name) values('" + messinfo.at(1) + "','" + messinfo.at(2) + "')");
        insertIntoFac.exec();
    }
    //InsertIntoFacAndMessTables(messinfo);
    QSqlQuery getId("select Facility_ID from Facility_Table where IP = '" + IP + "'");
    getId.exec();
    getId.next();
    QString id = getId.value(0).toString();

    //Insert info into DB
    QString Datetime = messinfo.at(0);
    QString Type = messinfo.at(3);
    QString Tag = messinfo.at(4);
    QString Message = messinfo.at(5);

    QSqlQuery insertOperation;
    insertOperation.exec("insert into Messages_Table(Datetime,Facility_ID,Type,Tag,Message,isNew) values('" + Datetime + "'," + id + ",'" + Type + "','" + Tag + "','" + Message + "',1)");
    emit NewRowInserted();
}
