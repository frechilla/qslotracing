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

#define MAX_LAPS    500

typedef QMap<quint32, quint8> PlayersMapRankingType;

namespace Ui {
    class StatsDialog;
}


class StatsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit StatsDialog(QWidget *parent = 0);
    ~StatsDialog();

    /// @brief Set the race finished flag, to manage the stats window
    /// @param Finished race flag
    void SetRaceFinished(bool bFinished);

    /// @brief Set player 1 data
    /// @param Player enabled flag
    /// @param Player name
    void SetPlayer1Data(bool bEnabled, QString name);

    /// @brief Set player 2 data
    /// @param Player enabled flag
    /// @param Player name
    void SetPlayer2Data(bool bEnabled, QString name);

    /// @brief Set player 3 data
    /// @param Player enabled flag
    /// @param Player name
    void SetPlayer3Data(bool bEnabled, QString name);

    /// @brief Set player 4 data
    /// @param Player enabled flag
    /// @param Player name
    void SetPlayer4Data(bool bEnabled, QString name);

    /// @brief Set player 5 data
    /// @param Player enabled flag
    /// @param Player name
    void SetPlayer5Data(bool bEnabled, QString name);

    /// @brief Set player 6 data
    /// @param Player enabled flag
    /// @param Player name
    void SetPlayer6Data(bool bEnabled, QString name);

    /// @brief Set Player 1 ranking data
    /// @param Player 1 ranking data map
    void SetRankingPlayer1(PlayersMapRankingType* mapValues);

    /// @brief Set Player 2 ranking data
    /// @param Player 2 ranking data map
    void SetRankingPlayer2(PlayersMapRankingType* mapValues);

    /// @brief Set Player 3 ranking data
    /// @param Player 3 ranking data map
    void SetRankingPlayer3(PlayersMapRankingType* mapValues);

    /// @brief Set Player 4 ranking data
    /// @param Player 4 ranking data map
    void SetRankingPlayer4(PlayersMapRankingType* mapValues);

    /// @brief Set Player 5 ranking data
    /// @param Player 5 ranking data map
    void SetRankingPlayer5(PlayersMapRankingType* mapValues);

    /// @brief Set Player 6 ranking data
    /// @param Player 6 ranking data map
    void SetRankingPlayer6(PlayersMapRankingType* mapValues);
private:
    /// @brief Dialog resource
    Ui::StatsDialog *ui;

    /// @brief Position plot
    QwtPlot* PositionPlot;

    /// @brief Player 1 curve
    QwtPlotCurve* curve1;

    /// @brief Player 2 curve
    QwtPlotCurve* curve2;

    /// @brief Player 3 curve
    QwtPlotCurve* curve3;

    /// @brief Player 4 curve
    QwtPlotCurve* curve4;

    /// @brief Player 5 curve
    QwtPlotCurve* curve5;

    /// @brief Player 6 curve
    QwtPlotCurve* curve6;

    /// @brief X values for player 1
    double xval1[MAX_LAPS];

    /// @brief Y values for player 1
    double yval1[MAX_LAPS];

    /// @brief X values for player 2
    double xval2[MAX_LAPS];

    /// @brief Y values for player 2
    double yval2[MAX_LAPS];

    /// @brief X values for player 3
    double xval3[MAX_LAPS];

    /// @brief Y values for player 3
    double yval3[MAX_LAPS];

    /// @brief X values for player 4
    double xval4[MAX_LAPS];

    /// @brief Y values for player 4
    double yval4[MAX_LAPS];

    /// @brief X values for player 5
    double xval5[MAX_LAPS];

    /// @brief Y values for player 5
    double yval5[MAX_LAPS];

    /// @brief X values for player 6
    double xval6[MAX_LAPS];

    /// @brief Y values for player 6
    double yval6[MAX_LAPS];

    /// @brief Players enabled flag
    bool m_PlayersEnables[6];

    /// @brief Players names
    QString m_PlayersName[6];

    QTabWidget *tabOne;
    QTabWidget *tabTwo;

private slots:
    void on_tabWidget_currentChanged(int index);
};

#endif // STATSDIALOG_H
