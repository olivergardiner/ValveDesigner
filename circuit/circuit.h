#pragma once

#include "valvemodel/device.h"
#include <QtCharts/QChart>

#include <QChartView>
#include <QLineSeries>
#include <QTextBrowser>

#include <tuple>

enum eCircuit {
    TRIODE_COMMON_CATHODE,
    PENTODE_COMMON_CATHODE,
    AC_CATHODE_FOLLOWER,
    DC_CATHODE_FOLLOWER,
    LTP_PHASE_SPLITTER,
    CATHODINE_PHASE_SPLITTER,
    TRIODE_SE_OUTPUT,
    PENTODE_SE_OUTPUT,
    PENTODE_SE_OUTPUT_UL,
    TRIODE_PP_OUTPUT,
    PENTODE_PP_OUTPUT,
    PENTODE_PP_OUTPUT_UL
};

class Designer;

class Circuit
{
public:
    Circuit(Designer *designer);

    virtual void showAnodeCharacteristics(QChartView *chartView) = 0;
    virtual void calculateOperatingPoint() = 0;
    virtual void calculateOperatingPoint(QChartView *chartView) = 0;
    virtual void showLoadLines(QChartView *chartView) = 0;
    virtual void showDescription(QTextBrowser *textBrowser) = 0;

    void updateParameters(QLabel *labels[], QLineEdit *values[]);
    void parameterChanged(int index, QString value);

    void setDevice(Device *newDevice);

    static double interval(double maxValue);
    static int tickCount(double maxValue);

protected:
    void plotAnode(QChartView *chartView);
    void plotAnode(QChartView *chartView, double vg2);
    void plotAnode(QChartView *chartView, double vht, double ulTap);
    void plotScreen(QChartView *chartView, double vg2);
    void plotAnodePower(QChartView *chartView);
    void plotScreenPower(QChartView *chartView);
    void anodeAxes(QChartView *chartView);
    std::tuple<QPointF, int, int> intersection(QLineSeries *series1, QLineSeries *series2);

    Designer *designer;
    Device *device;
    Parameter *parameter[16];
    int nParameters;

    QLineSeries *anodeLoadLine = nullptr;
    QLineSeries *cathodeLoadLine = nullptr;
    QLineSeries *anodePowerLimit = nullptr;
    QLineSeries *screenPowerLimit = nullptr;
    QLineSeries *screenLL = nullptr;
    QLineSeries *screenCLL = nullptr;
    QLineSeries *anodeLL = nullptr;
    QLineSeries *cathodeLL = nullptr;
};
