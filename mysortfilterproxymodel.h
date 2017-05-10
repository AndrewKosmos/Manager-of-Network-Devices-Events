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
    QDateTime filterMinimumDateTime() const { return minDate; }
    void setFilterMinimumDate(const QDateTime &date);

    QDateTime filterMaximumDateTime() const { return maxDate; }
    void setFilterMaximumDate(const QDateTime &date);

    bool dateIsActive;
    bool txtFilterIsActive;
    int filterVariant;

    void RefreshFilter();
protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;
    bool lessThan(const QModelIndex &left, const QModelIndex &right) const override;

private:
    bool dateInRange(const QDateTime &date) const;
    ulong getIPweight(QString IP) const;

    QDateTime minDate;
    QDateTime maxDate;
};

#endif // MYSORTFILTERPROXYMODEL_H
