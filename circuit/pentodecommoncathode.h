#pragma once

#include "circuit.h"

#include <QChartView>

enum ePentodeCommonCathodeParameter {
    PEN_CC_VB,
    PEN_CC_RK,
    PEN_CC_RA,
    PEN_CC_RS,
    PEN_CC_RL,
    PEN_CC_IA,
    PEN_CC_VA,
    PEN_CC_IS,
    PEN_CC_VS,
    PEN_CC_IK,
    PEN_CC_VK,
    PEN_CC_GM,
    PEN_CC_GAIN,
    PEN_CC_GAIN_B
};

class PentodeCommonCathode : public Circuit
{
public:
    PentodeCommonCathode(Designer *designer);

    void showAnodeCharacteristics(QChartView *chartView);
    void calculateOperatingPoint();
    void calculateOperatingPoint(QChartView *chartView);
    void showLoadLines(QChartView *chartView);
    void showDescription(QTextBrowser *textBrowser);

private:
    void calculateOperatingPoint(QChartView *chartView, bool showChart, double ratio);
};
