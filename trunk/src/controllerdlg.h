#ifndef CONTROLLERDLG_H
#define CONTROLLERDLG_H

#include <QDialog>

namespace Ui {
    class ControllerDlg;
}

class ControllerDlg : public QDialog
{
    Q_OBJECT

public:
    explicit ControllerDlg(QWidget *parent = 0);
    ~ControllerDlg();

private:
    Ui::ControllerDlg *ui;
};

#endif // CONTROLLERDLG_H
