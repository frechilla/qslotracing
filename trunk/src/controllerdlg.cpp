#include "controllerdlg.h"
#include "ui_controllerdlg.h"

ControllerDlg::ControllerDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ControllerDlg)
{
    ui->setupUi(this);
}

ControllerDlg::~ControllerDlg()
{
    delete ui;
}
