#ifndef CUSTOMSQLMODEL_H
#define CUSTOMSQLMODEL_H
#include <QSqlQueryModel>
#include "databaseworker.h"

class CustomSqlModel : public QSqlQueryModel
{
    Q_OBJECT
public:
    CustomSqlModel(QObject *parent = 0);
    QVariant data(const QModelIndex &item, int role) const override;
    void refresh();
private:
    QStringList messageTypesList;
    QStringList messageColorsList;
};

#endif // CUSTOMSQLMODEL_H
