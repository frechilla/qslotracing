#ifndef STATSDIALOG_H
#define STATSDIALOG_H

#include <QDialog>

#include <qwt_scale_map.h>
#include <qwt_plot_curve.h>
#include <qwt_symbol.h>
#include <qwt_math.h>
#include <qcolor.h>
#include <qpainter.h>


const int Size = 27;

namespace Ui {
    class StatsDialog;
}

class StatsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit StatsDialog(QWidget *parent = 0);
    ~StatsDialog();
protected:
    virtual void paintEvent(QPaintEvent *);

private:
    Ui::StatsDialog *ui;

    QwtScaleMap xMap;
    QwtScaleMap yMap;
    QwtPlotCurve curve;
    double xval[Size];
    double yval[Size];

    void DrawContents(QPainter* painter);

private slots:
    void on_tabWidget_currentChanged(int index);
};

#endif // STATSDIALOG_H
