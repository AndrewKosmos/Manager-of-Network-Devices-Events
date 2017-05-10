#include "filtersettingswindow.h"
#include "ui_filtersettingswindow.h"
#include <QComboBox>
#include <QWidget>
#include <QLabel>
#include <QDateTimeEdit>
#include <QPushButton>

#include <QDebug>

FilterSettingsWindow::FilterSettingsWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FilterSettingsWindow)
{
    ui->setupUi(this);

    filterByCMB = new QComboBox();
    filterByCMB->insertItem(0,"Без фильтра");
    filterByCMB->insertItem(1,"По дате и времени");
    filterByCMB->insertItem(2,"По IP адресу");
    filterByCMB->insertItem(3,"По названию устройства");
    filterByCMB->insertItem(4,"По типу сообщения");
    filterByCMB->insertItem(5,"По тэгу сообщения");
    filterByCMB->insertItem(6,"По сообщению");
    filterByCMB->insertItem(7,"Смешанный");
    QLabel *filterByLabel = new QLabel(tr("&Выберите способ фильтрации записей:"));
    filterByLabel->setBuddy(filterByCMB);
    ui->FilterverticalLayout->addWidget(filterByLabel);
    ui->FilterverticalLayout->addWidget(filterByCMB);

    connect(filterByCMB,SIGNAL(currentIndexChanged(int)),this,SLOT(ComboboxStateChanged()));

    fw = new FilterWidget(this);
    QLabel *filterPatternLabel = new QLabel(tr("&Фильтр:"));
    filterPatternLabel->setBuddy(fw);
    ui->FilterverticalLayout->addWidget(filterPatternLabel);
    ui->FilterverticalLayout->addWidget(fw);

    FromDateDTE = new QDateTimeEdit();
    QLabel *FromDateLabel = new QLabel(tr("&Начальные дата и время:"));
    FromDateLabel->setBuddy(FromDateDTE);
    ui->FilterverticalLayout->addWidget(FromDateLabel);
    ui->FilterverticalLayout->addWidget(FromDateDTE);

    ToDateDTE = new QDateTimeEdit();
    QLabel *ToDateLabel = new QLabel(tr("&Конечные дата и время:"));
    ToDateLabel->setBuddy(ToDateDTE);
    ui->FilterverticalLayout->addWidget(ToDateLabel);
    ui->FilterverticalLayout->addWidget(ToDateDTE);

    applyFilterBtn = new QPushButton();
    applyFilterBtn->setText("Применить фильтр");
    applyFilterBtn->setObjectName("applyFilterBtn");
    applyFilterBtn->setIcon(QIcon(":/images/applyPNG"));
    ui->FilterverticalLayout->addWidget(applyFilterBtn);

    connect(applyFilterBtn,SIGNAL(clicked(bool)),this,SLOT(applyFilterBtn_clicked()));

    fw->setEnabled(false);
    FromDateDTE->setEnabled(false);
    ToDateDTE->setEnabled(false);

    /*QWidget* empty = new QWidget();
    empty->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);
    ui->FilterverticalLayout->addWidget(empty);*/
}

FilterSettingsWindow::~FilterSettingsWindow()
{
    delete ui;
}

void FilterSettingsWindow::ComboboxStateChanged()
{
    switch (filterByCMB->currentIndex()) {
    case 0:
        fw->clear();
        fw->setEnabled(false);
        FromDateDTE->clear();
        FromDateDTE->setEnabled(false);
        ToDateDTE->clear();
        ToDateDTE->setEnabled(false);
        break;
    case 1:
        fw->clear();
        fw->setEnabled(false);
        FromDateDTE->setEnabled(true);
        ToDateDTE->setEnabled(true);
        break;
    case 7:
        fw->setEnabled(true);
        FromDateDTE->setEnabled(true);
        ToDateDTE->setEnabled(true);
        break;
    default:
        fw->setEnabled(true);
        FromDateDTE->setEnabled(false);
        ToDateDTE->setEnabled(false);
        break;
    }
}

void FilterSettingsWindow::applyFilterBtn_clicked()
{
    /*switch (filterByCMB->currentIndex()) {
    case 0:
        qDebug() << "selected 0";
        break;
    case 1:
        qDebug() << "selected 1";
        break;
    case 2:
        qDebug() << "selected 2";
        break;
    default:
        break;
    }*/
    emit FilterButonClicked();
}
