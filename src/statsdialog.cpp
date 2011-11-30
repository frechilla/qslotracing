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
    m_PlayersEnabled[0] = false;
    m_PlayersEnabled[1] = false;
    m_PlayersEnabled[2] = false;
    m_PlayersEnabled[3] = false;
    m_PlayersEnabled[4] = false;
    m_PlayersEnabled[5] = false;

    // Initialize first position of data arrays
    xval1[0] = 0;
    yval1[1] = 1;
    xval2[0] = 0;
    yval2[1] = 2;
    xval3[0] = 0;
    yval3[1] = 3;
    xval4[0] = 0;
    yval4[1] = 4;
    xval5[0] = 0;
    yval5[1] = 5;
    xval6[0] = 0;
    yval6[1] = 6;

    tabOne = new QTabWidget;

    PositionPlot = new QwtPlot(QwtText("Position"));
    PositionPlot->insertLegend(new QwtLegend(), QwtPlot::RightLegend);

    PositionPlot->setFixedSize(900,600);
    PositionPlot->setAxisScale(QwtPlot::xBottom,1, 10);
    PositionPlot->setAxisTitle(QwtPlot::xBottom,"Vueltas");
    PositionPlot->setAxisScale(QwtPlot::yLeft,6,1);
    PositionPlot->setAxisTitle(QwtPlot::yLeft,"Posicion");

    tabOne->addTab(PositionPlot, tr("Position stats"));

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
    m_PlayersEnabled[0] = true;

    // Set player name
    m_PlayersName[0] = name;
}

void StatsDialog::SetPlayer2Data(bool bEnabled, QString name)
{
    // Set player flag
    m_PlayersEnabled[1] = true;

    // Set player name
    m_PlayersName[1] = name;
}

void StatsDialog::SetPlayer3Data(bool bEnabled, QString name)
{
    // Set player flag
    m_PlayersEnabled[2] = true;

    // Set player name
    m_PlayersName[2] = name;
}

void StatsDialog::SetPlayer4Data(bool bEnabled, QString name)
{
    // Set player flag
    m_PlayersEnabled[3] = true;

    // Set player name
    m_PlayersName[3] = name;
}

void StatsDialog::SetPlayer5Data(bool bEnabled, QString name)
{
    // Set player flag
    m_PlayersEnabled[4] = true;

    // Set player name
    m_PlayersName[4] = name;
}

void StatsDialog::SetPlayer6Data(bool bEnabled, QString name)
{
    // Set player flag
    m_PlayersEnabled[5] = true;

    // Set player name
    m_PlayersName[5] = name;
}

void StatsDialog::SetRankingPlayer1(PlayersMapRankingType* mapValues)
{
    PlayersMapRankingType::iterator it;

    // Iterate over ranking values
    it = mapValues->begin();
    while (it != mapValues->end())
    {
        // Store values
        xval1[it.key()] = it.key();
        yval1[it.key()] = it.value();

        // Next item
        it++;
    }

    // Create player 1 curve
    curve1 = new QwtPlotCurve(m_PlayersName[0]);

    // Attach data to player 1 curve
    curve1->setRawSamples(xval1, yval1, mapValues->size());

    // Configurate curve
    curve1->setPen(QPen(QColor::fromRgb(20, 125, 190), 3));
    curve1->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);

    // Attach curve to plot and draw
    curve1->attach(PositionPlot);
    PositionPlot->replot();
}

void StatsDialog::SetRankingPlayer2(PlayersMapRankingType* mapValues)
{
    PlayersMapRankingType::iterator it;

    // Iterate over ranking values
    it = mapValues->begin();
    while (it != mapValues->end())
    {
        // Store values
        xval2[it.key()] = it.key();
        yval2[it.key()] = it.value();

        // Next item
        it++;
    }

    // Create player 2 curve
    curve2 = new QwtPlotCurve(m_PlayersName[1]);

    // Attach data to player 2 curve
    curve2->setRawSamples(xval2, yval2, mapValues->size());

    // Configurate curve
    curve2->setPen(QPen(QColor::fromRgb(220, 225, 90), 3));
    curve2->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);

    // Attach curve to plot and draw
    curve2->attach(PositionPlot);
    PositionPlot->replot();
}

void StatsDialog::SetRankingPlayer3(PlayersMapRankingType* mapValues)
{
    PlayersMapRankingType::iterator it;

    // Iterate over ranking values
    it = mapValues->begin();
    while (it != mapValues->end())
    {
        // Store values
        xval3[it.key()] = it.key();
        yval3[it.key()] = it.value();

        // Next item
        it++;
    }

    // Create player 3 curve
    curve3 = new QwtPlotCurve(m_PlayersName[2]);

    // Attach data to player 3 curve
    curve3->setRawSamples(xval3, yval3, mapValues->size());

    // Configurate curve
    curve3->setPen(QPen(QColor::fromRgb(220, 225, 90), 3));
    curve3->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);

    // Attach curve to plot and draw
    curve3->attach(PositionPlot);
    PositionPlot->replot();
}

void StatsDialog::SetRankingPlayer4(PlayersMapRankingType* mapValues)
{
    PlayersMapRankingType::iterator it;

    // Iterate over ranking values
    it = mapValues->begin();
    while (it != mapValues->end())
    {
        // Store values
        xval4[it.key()] = it.key();
        yval4[it.key()] = it.value();

        // Next item
        it++;
    }

    // Create player 4 curve
    curve4 = new QwtPlotCurve(m_PlayersName[3]);

    // Attach data to player 4 curve
    curve4->setRawSamples(xval4, yval4, mapValues->size());

    // Configurate curve
    curve4->setPen(QPen(QColor::fromRgb(220, 225, 90), 3));
    curve4->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);

    // Attach curve to plot and draw
    curve4->attach(PositionPlot);
    PositionPlot->replot();
}

void StatsDialog::SetRankingPlayer5(PlayersMapRankingType* mapValues)
{
    PlayersMapRankingType::iterator it;

    // Iterate over ranking values
    it = mapValues->begin();
    while (it != mapValues->end())
    {
        // Store values
        xval5[it.key()] = it.key();
        yval5[it.key()] = it.value();

        // Next item
        it++;
    }

    // Create player 5 curve
    curve5 = new QwtPlotCurve(m_PlayersName[4]);

    // Attach data to player 5 curve
    curve5->setRawSamples(xval5, yval5, mapValues->size());

    // Configurate curve
    curve5->setPen(QPen(QColor::fromRgb(220, 225, 90), 3));
    curve5->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);

    // Attach curve to plot and draw
    curve5->attach(PositionPlot);
    PositionPlot->replot();
}

void StatsDialog::SetRankingPlayer6(PlayersMapRankingType* mapValues)
{
    PlayersMapRankingType::iterator it;

    // Iterate over ranking values
    it = mapValues->begin();
    while (it != mapValues->end())
    {
        // Store values
        xval6[it.key()] = it.key();
        yval6[it.key()] = it.value();

        // Next item
        it++;
    }

    // Create player 6 curve
    curve6 = new QwtPlotCurve(m_PlayersName[5]);

    // Attach data to player 6 curve
    curve6->setRawSamples(xval6, yval6, mapValues->size());

    // Configurate curve
    curve6->setPen(QPen(QColor::fromRgb(220, 225, 90), 3));
    curve6->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);

    // Attach curve to plot and draw
    curve6->attach(PositionPlot);
    PositionPlot->replot();
}

void StatsDialog::SetRankingData(MapRankingsType_t* mapValues, bool flags[])
{
    MapRankingsType_t::iterator itRanking;
    PlayerPosType_t::iterator   itLap;
    PlayerPosType_t             lapMap;
    quint16             curLap;
    quint8              curPos;
    quint8              curPlayer;

    itRanking = mapValues->begin();
    while (itRanking != mapValues->end())
    {
        // Get current lap
        curLap = itRanking.key();

        // Get current lap data
        lapMap = itRanking.value();

        // Iterate through lap data
        itLap = lapMap.begin();
        while (itLap != lapMap.end())
        {
            curPlayer = itLap.key();
            curPos = itLap.value();

            // Update each player curve, checking each player flag
            if ((flags[0] == true) && (curPlayer == 0))
            {
                // Player 1
                // Store values
                xval1[curLap+1] = curLap;
                yval1[curLap+1] = curPos;

            }
            else if ((flags[1] == true) && (curPlayer == 1))
            {
                // Player 2
                xval2[curLap+1] = curLap;
                yval2[curLap+1] = curPos;
            }
            else if (flags[2] == true)
            {
                // Player 3
                xval3[curLap+1] = curLap;
                yval3[curLap+1] = curPos;
            }
            else if (flags[3] == true)
            {
                // Player 4
                xval4[curLap+1] = curLap;
                yval4[curLap+1] = curPos;
            }
            else if (flags[4] == true)
            {
                // Player 5
                xval5[curLap+1] = curLap;
                yval5[curLap+1] = curPos;
            }
            else if (flags[5] == true)
            {
                // Player 6
                xval6[curLap+1] = curLap;
                yval6[curLap+1] = curPos;
            }

            // Next item
            itLap++;
        }

        // Next item
        itRanking++;
    }
}

void StatsDialog::ConfigurePlot(bool flags[], quint32 laps)
{
    // Check each player flag before setting up curves

    // Configure Player 1
    if (flags[0] == true)
    {
        // Player 1
        // Create player 1 curve
        curve1 = new QwtPlotCurve(m_PlayersName[0]);

        // Attach data to player 1 curve
        curve1->setRawSamples(xval1, yval1, laps+2);

        // Configurate curve
        curve1->setPen(QPen(QColor::fromRgb(20, 125, 190), 3));
        curve1->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);

        // Attach curve to plot and draw
        curve1->attach(PositionPlot);
        PositionPlot->replot();
    }

    // Configure Player 2
    if (flags[1] == true)
    {
        // Player 2
        // Create player 2 curve
        curve2 = new QwtPlotCurve(m_PlayersName[1]);

        // Attach data to player 2 curve
        curve2->setRawSamples(xval2, yval2, laps+2);

        // Configurate curve
        curve2->setPen(QPen(QColor::fromRgb(220, 225, 90), 3));
        curve2->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);

        // Attach curve to plot and draw
        curve2->attach(PositionPlot);
        PositionPlot->replot();
    }

    // Configure Player 3
    if (flags[2] == true)
    {
        // Player 3
        // Create player 3 curve
        curve3 = new QwtPlotCurve(m_PlayersName[2]);

        // Attach data to player 3 curve
        curve3->setRawSamples(xval3, yval3, laps+2);

        // Configurate curve
        curve3->setPen(QPen(QColor::fromRgb(220, 225, 90), 3));
        curve3->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);

        // Attach curve to plot and draw
        curve3->attach(PositionPlot);
        PositionPlot->replot();
    }

    // Configure Player 4
    if (flags[3] == true)
    {
        // Player 4
        // Create player 4 curve
        curve4 = new QwtPlotCurve(m_PlayersName[3]);

        // Attach data to player 4 curve
        curve4->setRawSamples(xval4, yval4, laps+2);

        // Configurate curve
        curve4->setPen(QPen(QColor::fromRgb(220, 225, 90), 3));
        curve4->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);

        // Attach curve to plot and draw
        curve4->attach(PositionPlot);
        PositionPlot->replot();
    }

    // Configure Player 5
    if (flags[4] == true)
    {
        // Player 5
        // Create player 5 curve
        curve5 = new QwtPlotCurve(m_PlayersName[4]);

        // Attach data to player 5 curve
        curve5->setRawSamples(xval5, yval5, laps+2);

        // Configurate curve
        curve5->setPen(QPen(QColor::fromRgb(220, 225, 90), 3));
        curve5->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);

        // Attach curve to plot and draw
        curve5->attach(PositionPlot);
        PositionPlot->replot();
    }

    // Configure Player 6
    if (flags[5] == true)
    {
        // Player 6
        // Create player 6 curve
        curve6 = new QwtPlotCurve(m_PlayersName[5]);

        // Attach data to player 6 curve
        curve6->setRawSamples(xval6, yval6, laps+2);

        // Configurate curve
        curve6->setPen(QPen(QColor::fromRgb(220, 225, 90), 3));
        curve6->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);

        // Attach curve to plot and draw
        curve6->attach(PositionPlot);
        PositionPlot->replot();
    }
}
