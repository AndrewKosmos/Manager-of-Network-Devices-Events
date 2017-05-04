#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QCloseEvent>
#include "devicemanager.h"
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
    ~MainWindow();

private:
    Ui::MainWindow *ui;

protected:
    virtual void closeEvent(QCloseEvent *event);

private slots:
    void on_DeviceManagerButton_clicked();
    void RefreshTableView();

signals:
    void ProgramIsClosing();
};

#endif // MAINWINDOW_H
