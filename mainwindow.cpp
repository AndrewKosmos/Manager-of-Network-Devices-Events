#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "networking.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    /* Setup tablewidget */

    ui->MessagesTableWidget->setColumnCount(6);
    ui->MessagesTableWidget->setShowGrid(true);
    ui->MessagesTableWidget->setHorizontalHeaderLabels(QStringList() << "Time" << "IP" << "Facility" << "Type" << "Tag" << "Message");
    ui->MessagesTableWidget->setColumnWidth(0,150);
    ui->MessagesTableWidget->setColumnWidth(1,100);
    ui->MessagesTableWidget->horizontalHeader()->setStretchLastSection(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::GetParsedSyslogMess(QStringList messInfo,QString color)
{
    ui->MessagesTableWidget->insertRow(0);
    QTableWidgetItem *itemDateTime = new QTableWidgetItem(messInfo.at(0));
    QTableWidgetItem *itemIP = new QTableWidgetItem(messInfo.at(1));
    QTableWidgetItem *itemFac = new QTableWidgetItem(messInfo.at(2));
    QTableWidgetItem *itemPriority = new QTableWidgetItem(messInfo.at(3));
    QTableWidgetItem *itemTag = new QTableWidgetItem(messInfo.at(4));
    QTableWidgetItem *itemMessage = new QTableWidgetItem(messInfo.at(5));
    itemDateTime->setBackgroundColor(color);
    itemIP->setBackgroundColor(color);
    itemFac->setBackgroundColor(color);
    itemPriority->setBackgroundColor(color);
    itemTag->setBackgroundColor(color);
    itemMessage->setBackgroundColor(color);
    ui->MessagesTableWidget->setItem(0,0,itemDateTime);
    ui->MessagesTableWidget->setItem(0,1,itemIP);
    ui->MessagesTableWidget->setItem(0,2,itemFac);
    ui->MessagesTableWidget->setItem(0,3,itemPriority);
    ui->MessagesTableWidget->setItem(0,4,itemTag);
    ui->MessagesTableWidget->setItem(0,5,itemMessage);
}

void MainWindow::on_DeviceManagerButton_clicked()
{
    DeviceManager* dm = new DeviceManager(this);
    dm->setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint);
    dm->show();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    emit ProgramIsClosing();
    event->accept();
}
