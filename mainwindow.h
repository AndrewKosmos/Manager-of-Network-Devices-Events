#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QCloseEvent>
#include <QDateTime>
#include <QDateTimeEdit>
#include "devicemanager.h"
#include "filtersettingswindow.h"
#include "networking.h"
#include "customsqlmodel.h"
#include "databaseworker.h"
#include "mysortfilterproxymodel.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    CustomSqlModel *model;
    MySortFilterProxyModel *proxy;
    QRegExp *finalFilterRegexp;
    QDateTime *toDatetime;
    QDateTime *fromDatetime;
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    FilterSettingsWindow *filter_window;

protected:
    virtual void closeEvent(QCloseEvent *event);

private slots:
    void on_DeviceManagerButton_clicked();
    void RefreshTableView();
    void ApplyFilter();

    void on_FilterButton_clicked();

signals:
    void ProgramIsClosing();
};

#endif // MAINWINDOW_H
