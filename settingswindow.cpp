#include "settingswindow.h"
#include "ui_settingswindow.h"

SettingsWindow::SettingsWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SettingsWindow)
{
    ui->setupUi(this);

    Settings s("settings.ini");
    s.LoadSettings();
    pathLE = new QLineEdit(this);
    QLabel *pathLabel = new QLabel(tr("&Путь к БД:"));
    pathLabel->setBuddy(pathLE);
    pathLE->setText(s.databasePath);
    ui->horizontalLayout->addWidget(pathLabel);
    ui->horizontalLayout->addWidget(pathLE);

    changePath = new QPushButton();
    changePath->setText("Изменить путь");
    changePath->setObjectName("changePathBtn");
    ui->horizontalLayout->addWidget(changePath);
    connect(changePath,SIGNAL(clicked(bool)),this,SLOT(ChangePath()));

    saveSettingsPB = new QPushButton();
    saveSettingsPB->setText("Сохранить настройки");
    saveSettingsPB->setObjectName("saveSettingsBtn");
    ui->horizontalLayout_2->addWidget(saveSettingsPB);
    connect(saveSettingsPB,SIGNAL(clicked(bool)),this,SLOT(SaveSettings()));
}

SettingsWindow::~SettingsWindow()
{
    delete ui;
}

void SettingsWindow::ChangePath()
{
    QString dbPath = QFileDialog::getSaveFileName(this,tr("Save database path"),"",tr("Database (*.db);;All Files(*)"));
    pathLE->setText(dbPath);
}

void SettingsWindow::SaveSettings()
{
    Settings s("settings.ini");
    s.SaveSettings(pathLE->text());
}
