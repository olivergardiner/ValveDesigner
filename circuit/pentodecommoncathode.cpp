#include "pentodecommoncathode.h"
#include "spice/spice.h"
#include "designer.h"

#include <QScatterSeries>

PentodeCommonCathode::PentodeCommonCathode(Designer *designer)
    : Circuit(designer)
{
    parameter[nParameters++] = new Parameter("Supply Voltage:", 300.0, true);
    parameter[nParameters++] = new Parameter("Cathode Resistor:", 1000.0, true);
    parameter[nParameters++] = new Parameter("Anode Resistor:", 100000.0, true);
    parameter[nParameters++] = new Parameter("Screen Resistor:", 390000.0, true);
    parameter[nParameters++] = new Parameter("Load Resistance:", 1000000.0, true);
    parameter[nParameters++] = new Parameter("Anode Current:", 0.0, false);
    parameter[nParameters++] = new Parameter("Anode Voltage:", 0.0, false);
    parameter[nParameters++] = new Parameter("Screen Current:", 0.0, false);
    parameter[nParameters++] = new Parameter("Screen Voltage:", 0.0, false);
    parameter[nParameters++] = new Parameter("Cathode Current:", 0.0, false);
    parameter[nParameters++] = new Parameter("Bias Point (Vk):", 0.0, false);
    parameter[nParameters++] = new Parameter("Gm:", 0.0, false);
    parameter[nParameters++] = new Parameter("Gain:", 0.0, false);
    parameter[nParameters++] = new Parameter("Gain (Rk bypassed):", 0.0, false);
}

void PentodeCommonCathode::showAnodeCharacteristics(QChartView *chartView)
{
    plotAnode(chartView, parameter[PEN_CC_VS]->getValue());
}

void PentodeCommonCathode::calculateOperatingPoint()
{
    double vb = parameter[PEN_CC_VB]->getValue();
    double ra = parameter[PEN_CC_RA]->getValue();
    double rs = parameter[PEN_CC_RS]->getValue();
    double rk = parameter[PEN_CC_RK]->getValue();
    double rl = parameter[PEN_CC_RL]->getValue();

    // First find the operating point using ngspice

    double vs = 0.0;

    spice("source models/cc-pentode.cir");

    device->alterParameters();

    spice("reset");

    QString command = QString {"alter VB=%1"}.arg(vb);
    spice(command.toStdString().c_str());

    command = QString {"alter RK=%1"}.arg(rk);
    spice(command.toStdString().c_str());

    command = QString {"alter RS=%1"}.arg(rs);
    spice(command.toStdString().c_str());

    command = QString {"alter RA=%1"}.arg(ra);
    spice(command.toStdString().c_str());

    spice("bg_run");

    if (!waitSimulationEnd()) {
        // The simulation did not complete inside 2s
        return;
    }

    double vgBias = 0.0;
    pvector_info myvec = ngGet_Vec_Info(const_cast<char *>("V(3)"));
    if (myvec) {
        vgBias = myvec->v_realdata[0];
        parameter[PEN_CC_VK]->setValue(vgBias);
    }

    double vaBias = 0.0;
    myvec = ngGet_Vec_Info(const_cast<char *>("V(4)"));
    if (myvec) {
        vaBias = myvec->v_realdata[0];
        parameter[PEN_CC_VA]->setValue(vaBias);
    }

    myvec = ngGet_Vec_Info(const_cast<char *>("V(6)"));
    if (myvec) {
        vs = myvec->v_realdata[0];
        parameter[PEN_CC_VS]->setValue(vs);
    }

    myvec = ngGet_Vec_Info(const_cast<char *>("I(VM1)"));
    if (myvec) {
        parameter[PEN_CC_IA]->setValue(myvec->v_realdata[0] * 1000.0);
    }

    myvec = ngGet_Vec_Info(const_cast<char *>("I(VM2)"));
    if (myvec) {
        parameter[PEN_CC_IS]->setValue(myvec->v_realdata[0] * 1000.0);
    }

    myvec = ngGet_Vec_Info(const_cast<char *>("I(VM3)"));
    if (myvec) {
        parameter[PEN_CC_IK]->setValue(myvec->v_realdata[0] * 1000.0);
    }

    double gm = 10.0 * (device->anodeCurrent(vaBias, -(vgBias - 0.05), vs) - device->anodeCurrent(vaBias, -(vgBias + 0.05), vs));
    parameter[PEN_CC_GM]->setValue(gm);
    parameter[PEN_CC_GAIN]->setValue(ra * rl / ((ra + rl) * 1000.0 * (1.0 / gm + rk / 1000.0)));
    parameter[PEN_CC_GAIN_B]->setValue(gm * ra * rl / ((ra + rl) * 1000.0));
}

void PentodeCommonCathode::calculateOperatingPoint(QChartView *chartView)
{
    calculateOperatingPoint(chartView, false, 0.5);
    double ratio = parameter[PEN_CC_VA]->getValue() / parameter[PEN_CC_VB]->getValue();
    calculateOperatingPoint(chartView, true, ratio);
}

void PentodeCommonCathode::showLoadLines(QChartView *chartView)
{
    double rk = parameter[PEN_CC_RK]->getValue();
    double vaMax = device->getVaMax();
    double vg1Max = device->getVg1Max();

    /*screenCLL = new QLineSeries();
    screenCLL->setPen(QPen(Qt::magenta));

    for (int j = 1; j < 101; j++) {
        double vg1 = vg1Max * j / 100.0;
        double ik = vg1 * 1000.0 / rk;
        double vg2 = device->screenVoltage(ik, vaMax * 0.5, -vg1);
        double is = device->screenCurrent(vaMax * 0.5, -vg1, vg2);
        screenCLL->append(vg2, is);
    }

    screenCLL->setName("Screen Cathode LL");
    chartView->chart()->addSeries(screenCLL);
    QObject::connect(screenCLL, &QLineSeries::hovered, designer, &Designer::on_seriesHovered);

    screenLL = new QLineSeries();
    screenLL->setPen(QPen(Qt::black));

    double ig2 = parameter[PEN_CC_VB]->getValue() / (parameter[PEN_CC_RS]->getValue() + parameter[PEN_CC_RK]->getValue());
    screenLL->append(0.0, ig2 * 1000.0);
    screenLL->append(parameter[PEN_CC_VB]->getValue(), 0.0);

    screenLL->setName("Screen LL");
    chartView->chart()->addSeries(screenLL);
    QObject::connect(screenLL, &QLineSeries::hovered, designer, &Designer::on_seriesHovered);*/

    anodeLoadLine = new QLineSeries();
    anodeLoadLine->setPen(QPen(Qt::green));

    double ia = parameter[PEN_CC_VB]->getValue() / (parameter[PEN_CC_RA]->getValue() + parameter[PEN_CC_RK]->getValue());
    anodeLoadLine->append(0.0, ia * 1000.0);
    anodeLoadLine->append(parameter[PEN_CC_VB]->getValue(), 0.0);

    anodeLoadLine->setName("Anode LL");
    chartView->chart()->addSeries(anodeLoadLine);
    QObject::connect(anodeLoadLine, &QLineSeries::hovered, designer, &Designer::on_seriesHovered);

    cathodeLoadLine = new QLineSeries();
    cathodeLoadLine->setPen(QPen(Qt::magenta));

    double vg2 = parameter[PEN_CC_VS]->getValue();
    double is = parameter[PEN_CC_IS]->getValue();

    for (int j = 1; j < 101; j++) {
        double va = parameter[PEN_CC_VB]->getValue() * j / 100.0;
        double ia = device->anodeCurrent(va, -parameter[PEN_CC_VK]->getValue(), vg2);
        cathodeLoadLine->append(va, ia);
    }

    cathodeLoadLine->setName("Cathode LL");
    chartView->chart()->addSeries(cathodeLoadLine);
    QObject::connect(cathodeLoadLine, &QLineSeries::hovered, designer, &Designer::on_seriesHovered);

    anodeAxes(chartView);
}

void PentodeCommonCathode::showDescription(QTextBrowser *textBrowser)
{
    textBrowser->setSource(QUrl("circuits/pentodeCommonCathode.html"));
}

void PentodeCommonCathode::calculateOperatingPoint(QChartView *chartView, bool showChart, double ratio)
{
    double vb = parameter[PEN_CC_VB]->getValue();
    double ra = parameter[PEN_CC_RA]->getValue();
    double rs = parameter[PEN_CC_RS]->getValue();
    double rk = parameter[PEN_CC_RK]->getValue();
    double rl = parameter[PEN_CC_RL]->getValue();

    double vaMax = device->getVaMax();
    double vg1Max = device->getVg1Max();

    double vaTest = vaMax * ratio;

    screenCLL = new QLineSeries();
    QLineSeries *screenCLL2 = new QLineSeries();

    for (int j = 1; j < 101; j++) {
        double vg1 = vg1Max * j / 100.0;
        double ik = vg1 * 1000.0 / rk;
        double vg2 = device->screenVoltage(ik, vaTest, -vg1);
        double is = device->screenCurrent(vaTest, -vg1, vg2);
        double ia = device->anodeCurrent(vaTest, -vg1, vg2);
        screenCLL->append(vg2, is);
        screenCLL2->append(vg1, ia);
    }

    screenCLL->setName("Screen Cathode LL");

    screenLL = new QLineSeries();

    double ig2 = vb / (rs + rk);
    screenLL->append(0.0, ig2 * 1000.0);
    screenLL->append(vb, 0.0);

    screenLL->setName("Screen LL");

    auto [screenOperatingPoint, i , j] = intersection(screenLL, screenCLL);
    double vg2 = screenOperatingPoint.x();
    double is = screenOperatingPoint.y();
    double vg1Test = screenCLL2->at(j).x();
    double iaTest = device->anodeCurrent(vaTest, -vg1Test, vg2);
    double iaTest2 = screenCLL2->at(j).y();
    double screenRatio = is / iaTest;

    cathodeLL = new QLineSeries();
    cathodeLL->setPen(QPen(Qt::magenta));

    for (int j = 1; j < 101; j++) {
        double va = vb * j / 100.0;
        double ia = device->anodeCurrent(va, -vg1Test, vg2);
        cathodeLL->append(va, ia);
    }

    cathodeLL->setName("Cathode LL");

    anodeLL = new QLineSeries();
    anodeLL->setPen(QPen(Qt::green));

    double iaMax = vb / (ra + rk);
    anodeLL->append(0.0, iaMax * 1000.0);
    anodeLL->append(vb, 0.0);

    anodeLL->setName("Anode LL");

    auto [anodeOperatingPoint, m, n] = intersection(anodeLL, cathodeLL);
    double va = anodeOperatingPoint.x();
    double ia = anodeOperatingPoint.y();
    double ik = ia + is;
    double vg1 = ik * rk / 1000;

    parameter[PEN_CC_VS]->setValue(vg2);
    parameter[PEN_CC_IK]->setValue(ik);
    parameter[PEN_CC_VK]->setValue(vg1);
    parameter[PEN_CC_IA]->setValue(ia);
    parameter[PEN_CC_IS]->setValue(is);
    parameter[PEN_CC_VA]->setValue(va);

    double gm = 10.0 * (device->anodeCurrent(va, -(vg1 - 0.05), vg2) - device->anodeCurrent(va, -(vg1 + 0.05), vg2));
    parameter[PEN_CC_GM]->setValue(gm);
    parameter[PEN_CC_GAIN]->setValue(ra * rl / ((ra + rl) * 1000.0 * (1.0 / gm + rk / 1000.0)));
    parameter[PEN_CC_GAIN_B]->setValue(gm * ra * rl / ((ra + rl) * 1000.0));

    if (showChart) {
        chartView->chart()->addSeries(cathodeLL);
        QObject::connect(cathodeLL, &QLineSeries::hovered, designer, &Designer::on_seriesHovered);
        chartView->chart()->addSeries(anodeLL);
        QObject::connect(anodeLL, &QLineSeries::hovered, designer, &Designer::on_seriesHovered);

        QScatterSeries *operatingPointSeries = new QScatterSeries();
        operatingPointSeries->setName("Operating Point");
        operatingPointSeries->setMarkerShape(QScatterSeries::MarkerShapeCircle);
        operatingPointSeries->setMarkerSize(10.0);
        operatingPointSeries->setColor(Qt::red);
        operatingPointSeries->append(anodeOperatingPoint);
        chartView->chart()->addSeries(operatingPointSeries);
        QObject::connect(operatingPointSeries, &QLineSeries::hovered, designer, &Designer::on_seriesHovered);
    }

}
