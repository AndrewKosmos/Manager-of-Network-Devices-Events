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
    void ParseSyslogMess(QString message,QString IP);
    void GetParsedSyslogMess(QStringList messInfo);
    void on_DeviceManagerButton_clicked();

signals:
    void SyslogMessageParsed(QStringList messInfo);
};

#endif // MAINWINDOW_H
