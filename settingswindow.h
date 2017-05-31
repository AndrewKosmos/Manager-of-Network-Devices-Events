#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QWidget>
#include <settings.h>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QFileDialog>

namespace Ui {
class SettingsWindow;
}

class SettingsWindow : public QWidget
{
    Q_OBJECT

public:
    QLineEdit *pathLE;
    QPushButton *changePath;
    QPushButton *saveSettingsPB;

    explicit SettingsWindow(QWidget *parent = 0);
    ~SettingsWindow();

private:
    Ui::SettingsWindow *ui;

public slots:
    void ChangePath();
    void SaveSettings();
};

#endif // SETTINGSWINDOW_H
