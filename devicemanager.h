#ifndef DEVICEMANAGER_H
#define DEVICEMANAGER_H

#include <QWidget>
#include <QPushButton>
#include <QDebug>
#include <networklineedit.h>
#include "snmp.h"

namespace Ui {
class DeviceManager;
}

class DeviceManager : public QWidget
{
    Q_OBJECT

public:
    explicit DeviceManager(QWidget *parent = 0);
    NetworkLineEdit *nleStartIP;
    NetworkLineEdit *nleEndIP;
    QPushButton *SearchIPButton;
    ~DeviceManager();

private slots:
    void SearchIPButton_clicked();

    void on_RefreshButton_clicked();

public slots:
    void addMyItem (QString, QString, QString);
    void scanDevice();
    void setSyslogStatus(int row,int column);
    //void enableSyslog(QTreeWidgetItem * item, int column);

private:
    Ui::DeviceManager *ui;
};

#endif // DEVICEMANAGER_H
