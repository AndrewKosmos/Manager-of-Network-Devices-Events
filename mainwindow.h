#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include "devicemanager.h"
#include "networking.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    int TableRowCounter;

private:
    Ui::MainWindow *ui;

private slots:
    void GetParsedSyslogMess(QStringList messInfo,QString color);
    void on_DeviceManagerButton_clicked();
};

#endif // MAINWINDOW_H
