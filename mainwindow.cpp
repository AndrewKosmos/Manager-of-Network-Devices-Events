#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "networking.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    /* Setup tableview and model */
    model = new CustomSqlModel(this);
    model->setQuery("select Messages_Table.Datetime, Facility_Table.IP,Facility_Table.Name,Messages_Table.Type,Messages_Table.Tag,Messages_Table.Message from Messages_Table,Facility_Table Where Messages_Table.Facility_ID = Facility_Table.Facility_ID and Messages_table.isNew = 1");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("DateTime"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("IP"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Name"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Type"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Tag"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Message"));

    proxy = new MySortFilterProxyModel(this);
    proxy->setSourceModel(model);

    ui->MessagesTableView->setModel(proxy);
    ui->MessagesTableView->resizeColumnsToContents();
    ui->MessagesTableView->horizontalHeader()->setSectionResizeMode(5,QHeaderView::Stretch);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::RefreshTableView()
{
    model->refresh();
    proxy->setSourceModel(model);
    ui->MessagesTableView->resizeColumnsToContents();
    ui->MessagesTableView->horizontalHeader()->setSectionResizeMode(5,QHeaderView::Stretch);
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
