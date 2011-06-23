#ifndef STATSDIALOG_H
#define STATSDIALOG_H

#include <QDialog>

#include <qwt_scale_map.h>
#include <qwt_symbol.h>
#include <qwt_math.h>
#include <qwt_plot.h>
#include <qwt_plot_marker.h>
#include <qwt_plot_curve.h>
#include <qwt_legend.h>
#include <qwt_series_data.h>
#include <qwt_plot_canvas.h>
#include <qwt_plot_panner.h>
#include <qwt_plot_magnifier.h>
#include <qwt_text.h>

#include <qcolor.h>
#include <qpainter.h>

#include <QTabWidget>

const int Size = 100;

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
     QwtPlot plot1;
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
     QwtPlot* plot1;

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

    /// @brief Set player 1 data
    void SetPlayer1Data(bool bEnabled, QString name);

    /// @brief Set player 2 data
    void SetPlayer2Data(bool bEnabled, QString name);

    /// @brief Set player 3 data
    void SetPlayer3Data(bool bEnabled, QString name);

    /// @brief Set player 4 data
    void SetPlayer4Data(bool bEnabled, QString name);

    /// @brief Set player 5 data
    void SetPlayer5Data(bool bEnabled, QString name);

    /// @brief Set player 6 data
    void SetPlayer6Data(bool bEnabled, QString name);
private:
    Ui::StatsDialog *ui;
    QwtPlot* plot1;
    QwtPlotCurve* curve1;
    QwtPlotCurve* curve2;
    QwtPlotCurve* curve3;
    double xval[Size];
    double yval[Size];
    double xval2[Size];
    double yval2[Size];

    QTabWidget *tabOne;
    QTabWidget *tabTwo;

    /// @brief Players enabled flag
    bool m_PlayersEnables[6];

    /// @brief Players names
    QString m_PlayersName[6];


private slots:
    void on_tabWidget_currentChanged(int index);
};

#endif // STATSDIALOG_H
