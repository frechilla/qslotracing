#include "statsdialog.h"
#include "ui_statsdialog.h"

#include <QVBoxLayout>
#include <QLabel>



GeneralTab::GeneralTab(QWidget *parent)
     : QWidget(parent)
 {
    int i;

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


    QLabel *fileNameLabel = new QLabel(tr("File Name:"));
     QLabel *fileNameEdit = new QLabel(tr("File Name:"));

     QLabel *pathLabel = new QLabel(tr("Path:"));

     QLabel *sizeLabel = new QLabel(tr("Size:"));

     QVBoxLayout *mainLayout = new QVBoxLayout;
     mainLayout->addWidget(fileNameLabel);
     mainLayout->addWidget(fileNameEdit);
     mainLayout->addWidget(pathLabel);
     mainLayout->addWidget(sizeLabel);
     mainLayout->addStretch(1);
     setLayout(mainLayout);
 }

void GeneralTab::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
     painter.setClipRect(contentsRect());
     DrawContents(&painter);
}

void GeneralTab::DrawContents(QPainter* painter)
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

OtherTab::OtherTab(QWidget *parent)
     : QWidget(parent)
 {
    int i;

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


    QLabel *fileNameLabel = new QLabel(tr("Hola:"));
     QLabel *fileNameEdit = new QLabel(tr("prueba:"));

     QLabel *pathLabel = new QLabel(tr("otro:"));

     QLabel *sizeLabel = new QLabel(tr("TAB:"));

     QVBoxLayout *mainLayout = new QVBoxLayout;
     mainLayout->addWidget(fileNameLabel);
     mainLayout->addWidget(fileNameEdit);
     mainLayout->addWidget(pathLabel);
     mainLayout->addWidget(sizeLabel);
     mainLayout->addStretch(1);
     setLayout(mainLayout);
 }

void OtherTab::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
     painter.setClipRect(contentsRect());
     DrawContents(&painter);
}

void OtherTab::DrawContents(QPainter* painter)
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

StatsDialog::StatsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StatsDialog)
{
    ui->setupUi(this);


    tabOne = new QTabWidget;
    tabOne->addTab(new GeneralTab(), tr("General"));
    tabOne->addTab(new OtherTab(), tr("General"));


    QVBoxLayout *mainLayout = new QVBoxLayout;
         mainLayout->addWidget(tabOne);
         setLayout(mainLayout);
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



