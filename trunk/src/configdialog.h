#ifndef CONFIGDIALOG_H
#define CONFIGDIALOG_H

#include <QDialog>

namespace Ui {
    class ConfigDialog;
}

class ConfigDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConfigDialog(QWidget *parent = 0);
    ~ConfigDialog();
    void GetPlayer(QString &player, bool &flag, int &car, int PlayerId);
    void GetConfigured(bool &configured);
    void GetSerialPort(QString &port);

private:
    Ui::ConfigDialog *ui;
    int PlayersFlags[6];
    QString PlayersName[6];
    bool m_configured;
    QString m_port;

private slots:
    void on_EditName6_textChanged(QString str);
    void on_EditName5_textChanged(QString str);
    void on_EditName4_textChanged(QString str);
    void on_EditName3_textChanged(QString str);
    void on_EditName2_textChanged(QString str);
    void on_EditName1_textChanged(QString str);
    void on_EditName6_textEdited(QString );
    void on_EditName5_textEdited(QString );
    void on_EditName4_textEdited(QString );
    void on_EditName3_textEdited(QString );
    void on_EditName2_textEdited(QString );
    void on_EditName1_textEdited(QString );
    void on_buttonBox_accepted();
};

#endif // CONFIGDIALOG_H
