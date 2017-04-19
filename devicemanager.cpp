#include "devicemanager.h"
#include "ui_devicemanager.h"
#include "mainwindow.h"

DeviceManager::DeviceManager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DeviceManager)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose,true);
}

DeviceManager::~DeviceManager()
{
    delete ui;
}

void DeviceManager::on_SearchIPButton_clicked()
{
    
}
