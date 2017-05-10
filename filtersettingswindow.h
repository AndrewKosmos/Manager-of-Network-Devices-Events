#ifndef FILTERSETTINGSWINDOW_H
#define FILTERSETTINGSWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QComboBox>
#include "filterwidget.h"

namespace Ui {
class FilterSettingsWindow;
}

class FilterSettingsWindow : public QWidget
{
    Q_OBJECT

public:
    explicit FilterSettingsWindow(QWidget *parent = 0);
    ~FilterSettingsWindow();
    FilterWidget *fw;
    QDateTimeEdit *FromDateDTE;
    QDateTimeEdit *ToDateDTE;
    QPushButton *applyFilterBtn;
    QComboBox *filterByCMB;

private:
    Ui::FilterSettingsWindow *ui;

private slots:
    //void textFilterChanged();
    //void dateFilterChanged();
    void applyFilterBtn_clicked();
    void ComboboxStateChanged();
signals:
    void FilterButonClicked();
};

#endif // FILTERSETTINGSWINDOW_H
