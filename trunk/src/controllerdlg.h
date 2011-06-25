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

    void SetGas1(int value);
    void SetGas2(int value);
    void SetGas3(int value);
    void SetGas4(int value);
    void SetGas5(int value);
    void SetGas6(int value);

    void SetChange1(bool change);
    void SetChange2(bool change);
    void SetChange3(bool change);
    void SetChange4(bool change);
    void SetChange5(bool change);
    void SetChange6(bool change);
private:
    Ui::ControllerDlg *ui;

};

#endif // CONTROLLERDLG_H
