#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "networking.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    TableRowCounter = 0;

    /* Setup tablewidget */

    ui->MessagesTableWidget->setColumnCount(6);
    ui->MessagesTableWidget->setShowGrid(true);
    ui->MessagesTableWidget->setHorizontalHeaderLabels(QStringList() << "Time" << "IP" << "Facility" << "Priority" << "Tag" << "Message");
    ui->MessagesTableWidget->setColumnWidth(0,150);
    ui->MessagesTableWidget->setColumnWidth(1,100);
    ui->MessagesTableWidget->horizontalHeader()->setStretchLastSection(true);

    /* Setup network tasks */
    Networking *SYSLOGManager = new Networking(514);
    connect(SYSLOGManager,SIGNAL(DatagramReaded(QString,QString)),this,SLOT(ParseSyslogMess(QString,QString)));
    connect(this,SIGNAL(SyslogMessageParsed(QStringList)),this,SLOT(GetParsedSyslogMess(QStringList)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::ParseSyslogMess(QString message,QString IP)
{
    QString messageCopy = message;
    QStringList messageInfo;

    //get part after '>'
    QStringList neededPartMessageForParseList = messageCopy.split('>');
    QString neededPartMessageForParse = neededPartMessageForParseList.at(1);

    //get message text
    QRegExp twoPartsOfMessageInfoRx(".*:");
    QStringList twoPartsOfMessageInfo = neededPartMessageForParse.split(twoPartsOfMessageInfoRx,QString::SkipEmptyParts);

    //capture datetime
    QRegExp rxDateTime("(.*\\d+:\\d+:\\d+)");
    int pos = 0;
    QStringList DateTimeList;

    while((pos = rxDateTime.indexIn(neededPartMessageForParse,pos)) != -1)
    {
        DateTimeList << rxDateTime.cap(1);
        pos += rxDateTime.matchedLength();
    }

    //remove message text
    int index2 = neededPartMessageForParse.lastIndexOf(":");
    neededPartMessageForParse.chop(neededPartMessageForParse.length() - index2);
    //remove datetime
    neededPartMessageForParse.remove(rxDateTime);
    neededPartMessageForParse.trimmed();

    QStringList facAndInfoList = neededPartMessageForParse.split(' ',QString::SkipEmptyParts);
    QString Facility = facAndInfoList.at(0);
    QString InfoForSplit = facAndInfoList.at(1);
    QStringList InfoList = InfoForSplit.split('/',QString::SkipEmptyParts);
    QString Priority = InfoList.at(1);
    QString Tag = InfoList.at(2);

    //Parse IP address
    QRegExp rxIP("([0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3})");
    pos = 0;
    QString senderIP;

    while((pos = rxIP.indexIn(IP,pos)) != -1)
    {
        senderIP = rxIP.cap(1);
        pos += rxIP.matchedLength();
    }

    messageInfo.append(DateTimeList);
    messageInfo.append(senderIP);
    messageInfo.append(Facility);
    messageInfo.append(Priority);
    messageInfo.append(Tag);
    messageInfo.append(twoPartsOfMessageInfo);

    emit GetParsedSyslogMess(messageInfo);
}

void MainWindow::GetParsedSyslogMess(QStringList messInfo)
{
    ui->MessagesTableWidget->insertRow(TableRowCounter);
    QTableWidgetItem *itemDateTime = new QTableWidgetItem(messInfo.at(0));
    QTableWidgetItem *itemIP = new QTableWidgetItem(messInfo.at(1));
    QTableWidgetItem *itemFac = new QTableWidgetItem(messInfo.at(2));
    QTableWidgetItem *itemPriority = new QTableWidgetItem(messInfo.at(3));
    QTableWidgetItem *itemTag = new QTableWidgetItem(messInfo.at(4));
    QTableWidgetItem *itemMessage = new QTableWidgetItem(messInfo.at(5));

    ui->MessagesTableWidget->setItem(TableRowCounter,0,itemDateTime);
    ui->MessagesTableWidget->setItem(TableRowCounter,1,itemIP);
    ui->MessagesTableWidget->setItem(TableRowCounter,2,itemFac);
    ui->MessagesTableWidget->setItem(TableRowCounter,3,itemPriority);
    ui->MessagesTableWidget->setItem(TableRowCounter,4,itemTag);
    ui->MessagesTableWidget->setItem(TableRowCounter,5,itemMessage);
}

void MainWindow::on_DeviceManagerButton_clicked()
{
    DeviceManager* dm = new DeviceManager(this);
    dm->setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint);
    dm->show();
}
