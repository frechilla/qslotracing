#include "statsdialog.h"
#include "ui_statsdialog.h"

#include <QVBoxLayout>
#include <QLabel>



GeneralTab::GeneralTab(QWidget *parent)
     : QWidget(parent)
 {
    int i;

    xMap.setScaleInterval(0, Size);
    yMap.setScaleInterval(6, 1);

    //
    // Calculate values
    //

    for(i=0; i<Size;i++)
    {
        xval[i] = i;
        yval[i] = i;
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
    plot1 = new QwtPlot(QwtText("CppQwtExample1"));

    plot1->setGeometry(0,0,640,400);
     plot1->setAxisScale(QwtPlot::xBottom, 0.0,2.0 * M_PI);
     plot1->setAxisScale(QwtPlot::yLeft,-1.0,1.0);

     QwtPlotCurve sine("Sine");
     sine.attach(plot1);

}

void OtherTab::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
     painter.setClipRect(contentsRect());
     DrawContents(&painter);
}

void OtherTab::DrawContents(QPainter* painter)
{
    //plot1->show();
}

StatsDialog::StatsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StatsDialog)
{
    int i;

    ui->setupUi(this);


    tabOne = new QTabWidget;

    plot1 = new QwtPlot(QwtText("prueba"));
    plot1->insertLegend(new QwtLegend(), QwtPlot::RightLegend);

     plot1->setFixedSize(700,400);
     plot1->setAxisScale(QwtPlot::xBottom,0, 100);
     plot1->setAxisTitle(QwtPlot::xBottom,"Vueltas");
     plot1->setAxisScale(QwtPlot::yLeft,6,1);
     plot1->setAxisTitle(QwtPlot::yLeft,"Posicion");

     curve1 = new QwtPlotCurve("player 1");
     curve2 = new QwtPlotCurve("player 2");

     for (i=0;i<10;i++)
     {
         yval[i] = 1;
     }
     for (i=10;i<23;i++)
     {
         yval[i] = 3;
     }
     for (i=23;i<30;i++)
     {
         yval[i] = 2;
     }
     for (i=30;i<60;i++)
     {
         yval[i] = 4;
     }
     for (i=60;i<77;i++)
     {
         yval[i] = 3;
     }
     for (i=77;i<89;i++)
     {
         yval[i] = 2;
     }
     for (i=89;i<100;i++)
     {
         yval[i] = 1;
     }
     for(i=0; i<Size;i++)
     {
         xval[i] = i;
     }
     curve1->setRawSamples(xval, yval, Size);
     curve1->setPen(QPen(QColor::fromRgb(20, 125, 190), 3));

     curve1->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);

     curve1->attach(plot1);


     for (i=0;i<20;i++)
     {
         yval2[i] = 4;
     }
     for (i=20;i<32;i++)
     {
         yval2[i] = 2;
     }
     for (i=32;i<40;i++)
     {
         yval2[i] = 3;
     }
     for (i=40;i<63;i++)
     {
         yval2[i] = 1;
     }
     for (i=63;i<70;i++)
     {
         yval2[i] = 2;
     }
     for (i=70;i<91;i++)
     {
         yval2[i] = 6;
     }
     for (i=91;i<100;i++)
     {
         yval2[i] = 4;
     }
     for(i=0; i<Size;i++)
     {
         xval2[i] = i;
     }
     curve2->setRawSamples(xval2, yval2, Size);
     curve2->setPen(QPen(QColor::fromRgb(220, 125, 20), 3));
     curve2->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);

     curve2->attach(plot1);

     tabOne->addTab(plot1, tr("Prueba grafica"));
    //tabOne->addTab(new GeneralTab(), tr("General"));
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



