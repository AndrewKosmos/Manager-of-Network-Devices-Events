#ifndef DEVICEMANAGER_H
#define DEVICEMANAGER_H

#include <QWidget>

namespace Ui {
class DeviceManager;
}

class DeviceManager : public QWidget
{
    Q_OBJECT

public:
    explicit DeviceManager(QWidget *parent = 0);
    ~DeviceManager();

private slots:
    void on_SearchIPButton_clicked();

private:
    Ui::DeviceManager *ui;
};

#endif // DEVICEMANAGER_H
