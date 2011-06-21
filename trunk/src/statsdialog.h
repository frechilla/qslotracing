#ifndef STATSDIALOG_H
#define STATSDIALOG_H

#include <QDialog>

#include <qwt_scale_map.h>
#include <qwt_plot_curve.h>
#include <qwt_symbol.h>
#include <qwt_math.h>
#include <qcolor.h>
#include <qpainter.h>

#include <QTabWidget>

const int Size = 27;

namespace Ui {
    class StatsDialog;
}

class GeneralTab : public QWidget
 {
     Q_OBJECT

 public:
     GeneralTab(QWidget *parent = 0);
 protected:
     virtual void paintEvent(QPaintEvent *);
 private:
     void DrawContents(QPainter* painter);
     QwtScaleMap xMap;
     QwtScaleMap yMap;
     QwtPlotCurve curve;
     double xval[Size];
     double yval[Size];

 };

class OtherTab : public QWidget
 {
     Q_OBJECT

 public:
     OtherTab(QWidget *parent = 0);
 protected:
     virtual void paintEvent(QPaintEvent *);
 private:
     void DrawContents(QPainter* painter);
     QwtScaleMap xMap;
     QwtScaleMap yMap;
     QwtPlotCurve curve;
     double xval[Size];
     double yval[Size];

 };

class StatsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit StatsDialog(QWidget *parent = 0);
    ~StatsDialog();

    /// @brief Set the race finished flag, to manage the stats window
    void SetRaceFinished(bool bFinished);

private:
    Ui::StatsDialog *ui;

    QTabWidget *tabOne;
    QTabWidget *tabTwo;



private slots:
    void on_tabWidget_currentChanged(int index);
};

#endif // STATSDIALOG_H
