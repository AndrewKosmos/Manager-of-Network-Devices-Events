#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QCloseEvent>
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

private:
    Ui::MainWindow *ui;

protected:
    virtual void closeEvent(QCloseEvent *event);

private slots:
    void GetParsedSyslogMess(QStringList messInfo,QString color);
    void on_DeviceManagerButton_clicked();

signals:
    void ProgramIsClosing();
};

#endif // MAINWINDOW_H
