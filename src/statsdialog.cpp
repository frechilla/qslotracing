#include "statsdialog.h"
#include "ui_statsdialog.h"

#include <QVBoxLayout>
#include <QLabel>


StatsDialog::StatsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StatsDialog)
{


    ui->setupUi(this);

    // Initialize enabled players list
    m_PlayersEnables[0] = false;
    m_PlayersEnables[1] = false;
    m_PlayersEnables[2] = false;
    m_PlayersEnables[3] = false;
    m_PlayersEnables[4] = false;
    m_PlayersEnables[5] = false;

    tabOne = new QTabWidget;

    plot1 = new QwtPlot(QwtText("prueba"));
    plot1->insertLegend(new QwtLegend(), QwtPlot::RightLegend);

    plot1->setFixedSize(700,400);
    plot1->setAxisScale(QwtPlot::xBottom,1, 10);
    plot1->setAxisTitle(QwtPlot::xBottom,"Vueltas");
    plot1->setAxisScale(QwtPlot::yLeft,6,1);
    plot1->setAxisTitle(QwtPlot::yLeft,"Posicion");

    tabOne->addTab(plot1, tr("Prueba grafica"));

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

void StatsDialog::SetPlayer1Data(bool bEnabled, QString name)
{
    // Set player flag
    m_PlayersEnables[0] = true;

    // Set player name
    m_PlayersName[0] = name;
}

void StatsDialog::SetPlayer2Data(bool bEnabled, QString name)
{
    // Set player flag
    m_PlayersEnables[1] = true;

    // Set player name
    m_PlayersName[1] = name;
}

void StatsDialog::SetPlayer3Data(bool bEnabled, QString name)
{
    // Set player flag
    m_PlayersEnables[2] = true;

    // Set player name
    m_PlayersName[2] = name;
}

void StatsDialog::SetPlayer4Data(bool bEnabled, QString name)
{
    // Set player flag
    m_PlayersEnables[3] = true;

    // Set player name
    m_PlayersName[3] = name;
}

void StatsDialog::SetPlayer5Data(bool bEnabled, QString name)
{
    // Set player flag
    m_PlayersEnables[4] = true;

    // Set player name
    m_PlayersName[4] = name;
}

void StatsDialog::SetPlayer6Data(bool bEnabled, QString name)
{
    // Set player flag
    m_PlayersEnables[5] = true;

    // Set player name
    m_PlayersName[5] = name;
}

void StatsDialog::SetRankingPlayer1(PlayersMapRankingType* mapValues)
{
    PlayersMapRankingType::iterator it;
    int i;

    i = 0;
    it = mapValues->begin();
    while (it != mapValues->end())
    {
        xval[i] = it.key();
        yval[i] = it.value();
        it++;
        i++;
    }


    curve1 = new QwtPlotCurve(m_PlayersName[0]);

    curve1->setRawSamples(xval, yval, mapValues->size());
    curve1->setPen(QPen(QColor::fromRgb(20, 125, 190), 3));

    curve1->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);

    curve1->attach(plot1);
    plot1->replot();
}

void StatsDialog::SetRankingPlayer2(PlayersMapRankingType* mapValues)
{
    PlayersMapRankingType::iterator it;
    int i;

    i = 0;
    it = mapValues->begin();
    while (it != mapValues->end())
    {
        //qDebug()<<"$ 2 ["<<i<<"] "<<it.key()<<" "<<it.value();
        xval2[i] = it.key();
        yval2[i] = it.value();
        it++;
        i++;
    }

    curve2 = new QwtPlotCurve(m_PlayersName[1]);

    curve2->setRawSamples(xval2, yval2, mapValues->size());
    curve2->setPen(QPen(QColor::fromRgb(10, 225, 90), 3));

    curve2->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);

    curve2->attach(plot1);
    plot1->replot();
}
