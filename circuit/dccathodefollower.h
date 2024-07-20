#pragma once

#include "circuit.h"

enum eDCCathodeFollowerParameter {
    TRI_DC_CF_VB,
    TRI_DC_CF_RK,
    TRI_DC_CF_RA,
    TRI_DC_CF_RL,
    TRI_DC_CF_VK1,
    TRI_DC_CF_VA1,
    TRI_DC_CF_IA1,
    TRI_DC_CF_GAIN,
    TRI_DC_CF_VK2,
    TRI_DC_CF_IA2
};

class DCCathodeFollower : public Circuit
{
public:
    DCCathodeFollower(Designer *designer);

    void showAnodeCharacteristics(QChartView *chartView);
    void calculateOperatingPoint();
    void calculateOperatingPoint(QChartView *chartView);
    void showLoadLines(QChartView *chartView);
    void showDescription(QTextBrowser *textBrowser);

private:
    QLineSeries *followerLoadLine;
};
