#ifndef MYSORTFILTERPROXYMODEL_H
#define MYSORTFILTERPROXYMODEL_H
#include <QSortFilterProxyModel>
#include <QDate>
#include <QLocale>
#include <qmath.h>

class MySortFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    MySortFilterProxyModel(QObject *parent = 0);
    QDate filterMinimumDate() const { return minDate; }
    void setFilterMinimumDate(const QDate &date);

    QDate filterMaximumDate() const { return maxDate; }
    void setFilterMaximumDate(const QDate &date);

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;
    bool lessThan(const QModelIndex &left, const QModelIndex &right) const override;

private:
    bool dateInRange(const QDate &date) const;
    ulong getIPweight(QString IP) const;

    QDate minDate;
    QDate maxDate;
};

#endif // MYSORTFILTERPROXYMODEL_H
