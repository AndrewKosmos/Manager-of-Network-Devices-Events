#include "devicemanager.h"
#include "ui_devicemanager.h"
#include "mainwindow.h"
#include <QLabel>

DeviceManager::DeviceManager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DeviceManager)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose,true);
    nleStartIP = new NetworkLineEdit;
    QLabel *nleStartIPLabel = new QLabel(tr("&Начальный IP:"));
    nleStartIPLabel->setBuddy(nleStartIP);
    ui->gridLayout_4->addWidget(nleStartIPLabel);
    ui->gridLayout_4->addWidget(nleStartIP);
    nleEndIP = new NetworkLineEdit;
    QLabel *nleEndIPLabel = new QLabel(tr("&Конечный IP:"));
    nleEndIPLabel->setBuddy(nleEndIP);
    ui->gridLayout_4->addWidget(nleEndIPLabel);
    ui->gridLayout_4->addWidget(nleEndIP);
    SearchIPButton = new QPushButton("Поиск");
    SearchIPButton->setObjectName("SearchIPButton");
    ui->gridLayout_4->addWidget(SearchIPButton);
    connect(SearchIPButton,SIGNAL(clicked(bool)),this,SLOT(SearchIPButton_clicked()));
    ui->tableWidget->setColumnCount(3);
    ui->tableWidget->setHorizontalHeaderItem(0,new QTableWidgetItem("Имя устройства"));
    ui->tableWidget->setHorizontalHeaderItem(1,new QTableWidgetItem("IP"));
    ui->tableWidget->setHorizontalHeaderItem(2,new QTableWidgetItem("Статус Syslog"));
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    connect(ui->tableWidget,SIGNAL(cellDoubleClicked(int,int)),this,SLOT(setSyslogStatus(int,int)));

    ui->ChartsButton->setEnabled(false);
}

DeviceManager::~DeviceManager()
{
    delete ui;
}

void DeviceManager::SearchIPButton_clicked()
{
    ui->tableWidget->clear();
    for(int i = 0; i < ui->tableWidget->rowCount(); i++)
    {
        ui->tableWidget->removeRow(ui->tableWidget->currentRow());
    }
    ui->tableWidget->setRowCount(0);
    ui->tableWidget->setColumnCount(3);
    ui->tableWidget->setHorizontalHeaderItem(0,new QTableWidgetItem("Имя устройства"));
    ui->tableWidget->setHorizontalHeaderItem(1,new QTableWidgetItem("IP"));
    ui->tableWidget->setHorizontalHeaderItem(2,new QTableWidgetItem("Статус Syslog"));
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    scanDevice();
}

void DeviceManager::scanDevice()
{
    QStringList lstRange;

    if (!(nleStartIP->text().isEmpty()) && !(nleEndIP->text().isEmpty()))
        lstRange = nleStartIP->getRangeIP(nleEndIP);

    if (nleStartIP->getRange())
    {
        if (nleStartIP->getRange() > 255)
        {
            qDebug () << "Very big range!!!";
            return;
        }

        foreach (QString strIP, lstRange)
        {
            Snmp *snmp = new Snmp;
            snmp->setIP(strIP);
            snmp->setCommunity("public12");

            connect (snmp, SIGNAL (sigGetName(QString, QString, QString)),
                     this, SLOT (addMyItem(QString, QString, QString)));

            snmp->start();
        }
    }
}

void DeviceManager::addMyItem(QString name, QString ip, QString syslog)
{
    qDebug() << "Device founded";

    ui->tableWidget->insertRow(ui->tableWidget->rowCount());
    int newRow = ui->tableWidget->rowCount() - 1;
    ui->tableWidget->setItem(newRow,0,new QTableWidgetItem(name));
    ui->tableWidget->setItem(newRow,1,new QTableWidgetItem(ip));
    QTableWidgetItem *syslog_status;
    if(syslog == "1")
    {
        syslog_status = new QTableWidgetItem("Включен");
        syslog_status->setIcon(QIcon(":/images/applyPNG"));
    }
    else {
        syslog_status = new QTableWidgetItem("Выключен");
        syslog_status->setIcon(QIcon(":/images/clearSVG"));
    }
    ui->tableWidget->setItem(newRow,2,syslog_status);
}

void DeviceManager::on_RefreshButton_clicked()
{
    ui->tableWidget->clear();
    for(int i = 0; i < ui->tableWidget->rowCount(); i++)
    {
        ui->tableWidget->removeRow(ui->tableWidget->currentRow());
    }
    ui->tableWidget->setRowCount(0);
    ui->tableWidget->setColumnCount(3);
    ui->tableWidget->setHorizontalHeaderItem(0,new QTableWidgetItem("Имя устройства"));
    ui->tableWidget->setHorizontalHeaderItem(1,new QTableWidgetItem("IP"));
    ui->tableWidget->setHorizontalHeaderItem(2,new QTableWidgetItem("Статус Syslog"));
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    scanDevice();
}

void DeviceManager::setSyslogStatus(int row, int column)
{
    QTableWidgetItem *ipItem = ui->tableWidget->item(row,1);
    if(column == 2)
    {
        QTableWidgetItem *newItem = ui->tableWidget->item(row,column);
        if(newItem->text() == "Включен")
        {
            Snmp *snmp = new Snmp;
            snmp->setIP(ipItem->text());
            snmp->setCommunity("public12");
            snmp->start();
            snmp->setSyslogStatus(false);

            newItem->setText("Выключен");
            newItem->setIcon(QIcon(":/images/clearSVG"));
            ui->tableWidget->setItem(row,column,newItem);
        }
        else {
            Snmp *snmp = new Snmp;
            snmp->setIP(ipItem->text());
            snmp->setCommunity("public12");
            snmp->start();
            snmp->setSyslogStatus(true);
            newItem->setText("Включен");
            newItem->setIcon(QIcon(":/images/applyPNG"));
            ui->tableWidget->setItem(row,column,newItem);
        }
    }
}

void DeviceManager::on_ChartsButton_clicked()
{
    ChartsWindow *cw = new ChartsWindow(this,ipForSearch);
    cw->setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint);
    cw->show();
}

void DeviceManager::on_tableWidget_itemClicked(QTableWidgetItem *item)
{
    ui->ChartsButton->setEnabled(true);
    int row = item->row();
    QTableWidgetItem *getItem = ui->tableWidget->item(row,1);
    ipForSearch = getItem->text();
}
