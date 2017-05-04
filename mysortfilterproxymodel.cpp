#include "mysortfilterproxymodel.h"

MySortFilterProxyModel::MySortFilterProxyModel(QObject *parent) : QSortFilterProxyModel(parent){}

void MySortFilterProxyModel::setFilterMinimumDate(const QDate &date)
{
    minDate = date;
    invalidateFilter();
}

void MySortFilterProxyModel::setFilterMaximumDate(const QDate &date)
{
    maxDate = date;
    invalidateFilter();
}

bool MySortFilterProxyModel::filterAcceptsRow(int sourceRow,const QModelIndex &sourceParent) const
{

}

ulong MySortFilterProxyModel::getIPweight(QString IP) const
{
    QStringList ipGroups = IP.split('.');

    QString GroupOneString = ipGroups.at(0);
    QString GroupSecondString = ipGroups.at(1);
    QString GroupThirdString = ipGroups.at(2);
    QString GroupFourthString = ipGroups.at(3);

    long GroupOne = GroupOneString.toLong();
    long GroupSecond = GroupSecondString.toLong();
    long GroupThird = GroupThirdString.toLong();
    long GroupFourth = GroupFourthString.toLong();

    ulong IpWeight = (GroupOne * qPow(256,4)) + (GroupSecond * qPow(256,3)) + (GroupThird * qPow(256,2)) + (GroupFourth * 256);

    return IpWeight;
}

bool MySortFilterProxyModel::dateInRange(const QDate &date) const
{
    return (!minDate.isValid() || date > minDate)
            && (!maxDate.isValid() || date < maxDate);
}

bool MySortFilterProxyModel::lessThan(const QModelIndex &left,
                                      const QModelIndex &right) const
{
    QVariant leftData = sourceModel()->data(left);
    QVariant rightData = sourceModel()->data(right);
    QString leftString = leftData.toString();
    QString rightString = rightData.toString();

    if(left.column() == 0)
    {
        QLocale loc(QLocale::English);
        QDateTime dt_left = loc.toDateTime(leftString,"MMM dd yyyy HH:mm:ss");
        QDateTime dt_right = loc.toDateTime(rightString,"MMM dd yyyy HH:mm:ss");
        return dt_left < dt_right;
    }
    if(left.column() == 1)
    {
        //Sorting IP addresses
        return getIPweight(leftString) < getIPweight(rightString);
    }
    else {
        return QString::localeAwareCompare(leftString, rightString) < 0;
    }
}
