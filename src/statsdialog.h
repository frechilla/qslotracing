#ifndef STATSDIALOG_H
#define STATSDIALOG_H

#include <QDialog>

namespace Ui {
    class StatsDialog;
}

class StatsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit StatsDialog(QWidget *parent = 0);
    ~StatsDialog();

private:
    Ui::StatsDialog *ui;

private slots:
    void on_tabWidget_currentChanged(int index);
};

#endif // STATSDIALOG_H
