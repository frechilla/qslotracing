#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    // from QT documentation:
    //  simply tells the pre-compiler that this class has gui elements and
    //  needs to be run through the 'moc' you only need to add this to classes
    //  that use the signal/slot mechanism
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

private slots:
    void on_BtnConfigure_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_boton1_clicked();
    void on_pushButton_clicked();
};

#endif // MAINWINDOW_H
