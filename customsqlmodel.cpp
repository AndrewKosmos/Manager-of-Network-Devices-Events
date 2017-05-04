#include <QtWidgets>
#include "customsqlmodel.h"

CustomSqlModel::CustomSqlModel(QObject *parent) : QSqlQueryModel(parent){
    messageTypesList << "Emergency" << "Alert" << "Critical" << "Error" << "Warning" << "Notice" << "Informational" <<"Debug";
    messageColorsList << "#9AC0CD" << "#CD3333" << "#8B0000" << "red" << "yellow" << "lightGray" << "white" << "white";
}

QVariant CustomSqlModel::data(const QModelIndex &item, int role) const
{
    QVariant value = QSqlQueryModel::data(item, role);
    QString s,color;
    if(role == Qt::DisplayRole)
    {

    }


    if (role == Qt::BackgroundColorRole)
    {
        s = record(item.row()).value(3).toString();
        if(s == "Emergency")
        {
            color = "#9AC0CD";
        }
        if(s == "Alert")
        {
            color = "#CD3333";
        }
        if(s == "Critical")
        {
            color = "#8B0000";
        }
        if(s == "Error")
        {
            color = "red";
        }
        if(s == "Warning")
        {
            color = "yellow";
        }
        if(s == "Notice")
        {
            color = "lightGray";
        }
        if(s == "Informational" || s == "Debug"){
            color = "white";
        }

        return QVariant(QColor(color));
    }
    return value;
}

void CustomSqlModel::refresh()
{
    setQuery("select Messages_Table.Datetime, Facility_Table.IP,Facility_Table.Name,Messages_Table.Type,Messages_Table.Tag,Messages_Table.Message from Messages_Table,Facility_Table Where Messages_Table.Facility_ID = Facility_Table.Facility_ID and Messages_table.isNew = 1");
    setHeaderData(0, Qt::Horizontal, QObject::tr("DateTime"));
    setHeaderData(1, Qt::Horizontal, QObject::tr("IP"));
    setHeaderData(2, Qt::Horizontal, QObject::tr("Name"));
    setHeaderData(3, Qt::Horizontal, QObject::tr("Type"));
    setHeaderData(4, Qt::Horizontal, QObject::tr("Tag"));
    setHeaderData(5, Qt::Horizontal, QObject::tr("Message"));
}
