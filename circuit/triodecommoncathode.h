#pragma once

#include "circuit.h"

enum eTriodeCommonCathodeParameter {
    TRI_CC_VB,
    TRI_CC_RK,
    TRI_CC_RA,
    TRI_CC_RL,
    TRI_CC_VK,
    TRI_CC_VA,
    TRI_CC_IA,
//    TRI_CC_AR,
    TRI_CC_GAIN,
    TRI_CC_GAIN_B
};

class TriodeCommonCathode : public Circuit
{
public:
    TriodeCommonCathode(Designer *designer);

    void showAnodeCharacteristics(QChartView *chartView);
    void calculateOperatingPoint();
    void calculateOperatingPoint(QChartView *chartView);
    void showLoadLines(QChartView *chartView);
    void showDescription(QTextBrowser *textBrowser);
};
