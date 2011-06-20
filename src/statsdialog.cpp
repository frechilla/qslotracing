#include "statsdialog.h"
#include "ui_statsdialog.h"


StatsDialog::StatsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StatsDialog)
{
    int i;

    ui->setupUi(this);

    xMap.setScaleInterval(-0.5, 10.5);
    yMap.setScaleInterval(-1.1, 1.1);

    //
    // Calculate values
    //
    for(i=0; i<Size;i++)
    {
        xval[i] = double(i) * 10.0 / double(Size - 1);
        yval[i] = qSin(xval[i]) * qCos(2.0 * xval[i]);
    }

    curve.setSymbol(new QwtSymbol(QwtSymbol::Cross, Qt::NoBrush,
                                  QPen(Qt::black), QSize(5, 5) ) );
    curve.setPen(QColor(Qt::darkGreen));
    curve.setStyle(QwtPlotCurve::Lines);
    curve.setCurveAttribute(QwtPlotCurve::Fitted);

    curve.setRawSamples(xval, yval, Size);

}

StatsDialog::~StatsDialog()
{
    delete ui;
}

void StatsDialog::on_tabWidget_currentChanged(int index)
{
    int cual;

    cual = index;
    if (index == 0)
    {
    }
}

void StatsDialog::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
     painter.setClipRect(contentsRect());
     DrawContents(&painter);
}

void StatsDialog::DrawContents(QPainter* painter)
{
    int deltay;

    QRect r = contentsRect();

    deltay = r.height();

    r.setHeight(deltay);

    xMap.setPaintInterval(r.left(), r.right());
       yMap.setPaintInterval(r.top(), r.bottom());

       painter->setRenderHint(QPainter::Antialiasing,
           curve.testRenderHint(QwtPlotItem::RenderAntialiased) );
       curve.draw(painter, xMap, yMap, r);
}
