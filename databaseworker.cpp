#include "databaseworker.h"

DatabaseWorker::DatabaseWorker(QString path)
{
    messagesDatabase = QSqlDatabase::addDatabase("QSQLITE");
    messagesDatabase.setDatabaseName(path);
    if(QFile::exists(path))
    {
        messagesDatabase.setDatabaseName(path);
        messagesDatabase.open();
        QSqlQuery create_session_table("CREATE TABLE Session_Table (ID integer NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,Message_ID integer NOT NULL, FOREIGN KEY (Message_ID) REFERENCES Messages_Table (Message_ID))");
        create_session_table.exec();
    }
    else
    {
        messagesDatabase.setDatabaseName(path);
        messagesDatabase.open();
        QSqlQuery create_facility_table("CREATE TABLE Facility_Table (Facility_ID integer NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,IP varchar(15) NOT NULL,Name text NOT NULL UNIQUE)");
        create_facility_table.exec();
        QSqlQuery create_messages_table("CREATE TABLE Messages_Table (Message_ID integer NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,Facility_ID integer NOT NULL,Type text NOT NULL,Tag text NOT NULL,Message text NOT NULL, FOREIGN KEY (Facility_ID) REFERENCES Facility_Table (Facility_ID))");
        create_messages_table.exec();
        QSqlQuery create_session_table("CREATE TABLE Session_Table (ID integer NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,Message_ID integer NOT NULL, FOREIGN KEY (Message_ID) REFERENCES Messages_Table (Message_ID))");
        create_session_table.exec();
    }
}

void DatabaseWorker::DeleteSessionTable()
{
    QSqlQuery delete_session_table("DROP TABLE IF EXISTS Session_Table");
    delete_session_table.exec();
}
