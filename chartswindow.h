#ifndef CHARTSWINDOW_H
#define CHARTSWINDOW_H

#include <QWidget>
#include <QDebug>
#include "snmp.h"

namespace Ui {
class ChartsWindow;
}

class ChartsWindow : public QWidget
{
    Q_OBJECT

public:
    QString ip;
    explicit ChartsWindow(QWidget *parent = 0,QString ip_inner = NULL);
    ~ChartsWindow();

private:
    Ui::ChartsWindow *ui;
};

#endif // CHARTSWINDOW_H
