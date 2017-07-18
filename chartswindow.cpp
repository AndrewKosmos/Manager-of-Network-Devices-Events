#include "chartswindow.h"
#include "ui_chartswindow.h"
#include <QtCharts>

ChartsWindow::ChartsWindow(QWidget *parent,QString ip_inner) :
    QWidget(parent),
    ui(new Ui::ChartsWindow)
{
    ui->setupUi(this);
    ip = ip_inner;
    qDebug() << "IP FOR CHART: " << ip;

    Snmp *snmp = new Snmp;
    snmp->setIP(ip);
    snmp->setCommunity("public12");
    snmp->start();
    QString cpuUsagePerFiveSecond(snmp->getMetaInfo(ip,"public12","1.3.6.1.4.1.2011.6.3.4.1.2.0.0.0"));
    QString cpuUsagePerOneMinute(snmp->getMetaInfo(ip,"public12","1.3.6.1.4.1.2011.6.3.4.1.3.0.0.0"));
    QString cpuUsagePerFiveMinute(snmp->getMetaInfo(ip,"public12","1.3.6.1.4.1.2011.6.3.4.1.4.0.0.0"));

    QString memoryAll(snmp->getMetaInfo(ip,"public12","1.3.6.1.4.1.2011.6.3.5.1.1.2.0.0.0"));
    QString memoryUse(snmp->getMetaInfo(ip,"public12","1.3.6.1.4.1.2011.6.3.5.1.1.4.0.0.0"));
    QString memoryFree(snmp->getMetaInfo(ip,"public12","1.3.6.1.4.1.2011.6.3.5.1.1.3.0.0.0"));

    int memoryAllInt = memoryAll.toInt();
    int memoryUseInt = memoryUse.toInt();
    int memoryFreeInt = memoryFree.toInt();

    float memoryAllFloatMb = (memoryAllInt / 1024) / 1024 ;
    float memoryUseFloatMb = float((memoryUseInt / 1024) / 1024) ;
    float memoryFreeFloatMb = (memoryFreeInt / 1024) / 1024 ;
    qDebug() << memoryUseFloatMb << memoryFreeFloatMb;
    QBarSet *set0 = new QBarSet("Per Five Seconds");
    QBarSet *set1 = new QBarSet("Per One Minute");
    QBarSet *set2 = new QBarSet("Per Five Minutes");

        //*set0 << 1 << 2 << 3 << 4 << 5 << 6;
    //*set0 << cpuUsagePerFiveSecond.toInt() << cpuUsagePerOneMinute.toInt() << cpuUsagePerFiveMinute.toInt();
    *set0 << cpuUsagePerFiveSecond.toInt();
    *set1 << cpuUsagePerOneMinute.toInt();
    *set2 << cpuUsagePerFiveMinute.toInt();
    QBarSeries *series = new QBarSeries();
    series->append(set0);
    series->append(set1);
    series->append(set2);


    QChart *CPUchart = new QChart();
    CPUchart->addSeries(series);
    CPUchart->setTitle("Device CPU usage (in %)");
    CPUchart->setAnimationOptions(QChart::AllAnimations);

    QStringList categories;
    categories << "CPU Usage";
    QBarCategoryAxis *axis = new QBarCategoryAxis();
    axis->append(categories);
    CPUchart->createDefaultAxes();
    CPUchart->setAxisX(axis, series);

    CPUchart->legend()->setVisible(true);
    CPUchart->legend()->setAlignment(Qt::AlignBottom);
    QChartView *CPUchartView = new QChartView(CPUchart);
    CPUchartView->setRenderHint(QPainter::Antialiasing);
    ui->horizontalLayout->addWidget(CPUchartView);


    QBarSet *setUse = new QBarSet("Used Memory");
    QBarSet *setFree = new QBarSet("Free Memory");

    *setUse << memoryUseFloatMb;
    *setFree << memoryFreeFloatMb;
    QBarSeries *seriesMemory = new QBarSeries();
    seriesMemory->append(setUse);
    seriesMemory->append(setFree);


    QChart *Memorychart = new QChart();
    Memorychart->addSeries(seriesMemory);
    Memorychart->setTitle("Device Memory usage (in Mb)");
    Memorychart->setAnimationOptions(QChart::AllAnimations);

    QStringList categoriesMem;
    categoriesMem << "Memory Usage";
    QBarCategoryAxis *axisMemory = new QBarCategoryAxis();
    axisMemory->append(categoriesMem);
    Memorychart->createDefaultAxes();
    Memorychart->setAxisX(axisMemory, seriesMemory);

    Memorychart->legend()->setVisible(true);
    Memorychart->legend()->setAlignment(Qt::AlignBottom);
    QChartView *MemorychartView = new QChartView(Memorychart);
    MemorychartView->setRenderHint(QPainter::Antialiasing);
    ui->horizontalLayout->addWidget(MemorychartView);
}

ChartsWindow::~ChartsWindow()
{
    delete ui;
}
