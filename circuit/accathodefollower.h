#pragma once

#include "circuit.h"

enum eACCathodeFollowerParameter {
    TRI_CF_VB,
    TRI_CF_RB,
    TRI_CF_RK,
    TRI_CF_RL,
    TRI_CF_VBIAS,
    TRI_CF_VK,
    TRI_CF_IA,
    TRI_CF_RO
};

class ACCathodeFollower : public Circuit
{
public:
    ACCathodeFollower(Designer *designer);

    void showAnodeCharacteristics(QChartView *chartView);
    void calculateOperatingPoint();
    void calculateOperatingPoint(QChartView *chartView);
    void showLoadLines(QChartView *chartView);
    void showDescription(QTextBrowser *textBrowser);

private:
    QLineSeries *acLoadLine;
};
