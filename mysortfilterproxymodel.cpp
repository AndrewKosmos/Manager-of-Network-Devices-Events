#include "mysortfilterproxymodel.h"

MySortFilterProxyModel::MySortFilterProxyModel(QObject *parent) : QSortFilterProxyModel(parent){}

void MySortFilterProxyModel::setFilterMinimumDate(const QDateTime &date)
{
    minDate = date;
    //invalidateFilter();
}

void MySortFilterProxyModel::setFilterMaximumDate(const QDateTime &date)
{
    maxDate = date;
    //invalidateFilter();
}

void MySortFilterProxyModel::RefreshFilter()
{
    invalidateFilter();
}

bool MySortFilterProxyModel::filterAcceptsRow(int sourceRow,const QModelIndex &sourceParent) const
{
    QModelIndex index0 = sourceModel()->index(sourceRow, 0, sourceParent);
    QModelIndex index1 = sourceModel()->index(sourceRow, 1, sourceParent);
    QModelIndex index2 = sourceModel()->index(sourceRow, 2, sourceParent);
    QModelIndex index3 = sourceModel()->index(sourceRow, 3, sourceParent);
    QModelIndex index4 = sourceModel()->index(sourceRow, 4, sourceParent);
    QModelIndex index5 = sourceModel()->index(sourceRow, 5, sourceParent);

    if(dateIsActive == false && txtFilterIsActive == false)
    {
        return sourceModel()->data(index1).toString().contains(filterRegExp())
                || sourceModel()->data(index2).toString().contains(filterRegExp())
                || sourceModel()->data(index3).toString().contains(filterRegExp())
                || sourceModel()->data(index4).toString().contains(filterRegExp())
                || sourceModel()->data(index5).toString().contains(filterRegExp());
    }

    if(dateIsActive == true && txtFilterIsActive == false)
    {
        return dateInRange(sourceModel()->data(index0).toDateTime());
    }
    if(dateIsActive == true && txtFilterIsActive == true)
    {
        return (sourceModel()->data(index1).toString().contains(filterRegExp())
                || sourceModel()->data(index2).toString().contains(filterRegExp())
                || sourceModel()->data(index3).toString().contains(filterRegExp())
                || sourceModel()->data(index4).toString().contains(filterRegExp())
                || sourceModel()->data(index5).toString().contains(filterRegExp()))
                && dateInRange(sourceModel()->data(index0).toDateTime());
    }
    if(dateIsActive == false && txtFilterIsActive == true)
    {
        switch (filterVariant) {
        case 1:
            return sourceModel()->data(index1).toString().contains(filterRegExp());
            break;
        case 2:
            return sourceModel()->data(index2).toString().contains(filterRegExp());
            break;
        case 3:
            return sourceModel()->data(index3).toString().contains(filterRegExp());
            break;
        case 4:
            return sourceModel()->data(index4).toString().contains(filterRegExp());
            break;
        case 5:
            return sourceModel()->data(index5).toString().contains(filterRegExp());
            break;
        default:
            break;
        }
    }
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

bool MySortFilterProxyModel::dateInRange(const QDateTime &date) const
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
