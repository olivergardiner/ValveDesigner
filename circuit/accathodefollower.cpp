#include "accathodefollower.h"
#include "spice/spice.h"
#include "valvemodel/valvemodel.h"
#include "designer.h"

#include <QLineSeries>
#include <QScatterSeries>

ACCathodeFollower::ACCathodeFollower(Designer *designer)
    : Circuit(designer)
{
    parameter[nParameters++] = new Parameter("Supply Voltage:", 280.0, true);
    parameter[nParameters++] = new Parameter("Bias Resistor (Rb):", 680.0, true);
    parameter[nParameters++] = new Parameter("Load Resistor (Rk):", 47000.0, true);
    parameter[nParameters++] = new Parameter("Circuit Load (Rl):", 100000.0, true);
    parameter[nParameters++] = new Parameter("Bias Point (Vb):", 0.0, false);
    parameter[nParameters++] = new Parameter("Cathode Voltage:", 0.0, false);
    parameter[nParameters++] = new Parameter("Cathode Current:", 0.0, false);
    parameter[nParameters++] = new Parameter("Output Impedance:", 0.0, false);
}

void ACCathodeFollower::showAnodeCharacteristics(QChartView *chartView)
{
    plotAnode(chartView);
}

void ACCathodeFollower::calculateOperatingPoint()
{
}

void ACCathodeFollower::calculateOperatingPoint(QChartView *chartView)
{
    double vgMax = device->getVg1Max();
    double vb = parameter[TRI_CF_VB]->getValue();
    double rk = parameter[TRI_CF_RB]->getValue();
    double rl = parameter[TRI_CF_RL]->getValue();
    double ra = parameter[TRI_CF_RK]->getValue();

    anodeLoadLine = new QLineSeries();
    anodeLoadLine->setPen(QPen(Qt::green));

    double ia = vb / (ra + rk);
    anodeLoadLine->append(0.0, ia * 1000.0);
    anodeLoadLine->append(vb, 0.0);

    anodeLoadLine->setName("Anode LL");
    chartView->chart()->addSeries(anodeLoadLine);
    QObject::connect(anodeLoadLine, &QLineSeries::hovered, designer, &Designer::on_seriesHovered);

    cathodeLoadLine = new QLineSeries();
    cathodeLoadLine->setPen(QPen(Qt::magenta));

    for (int j = 1; j < 101; j++) {
        double vg = vgMax * j / 100.0;
        double ia = vg * 1000.0 / rk;
        double va = device->anodeVoltage(ia, -vg);
        cathodeLoadLine->append(va, ia);
    }

    cathodeLoadLine->setName("Cathode LL");
    chartView->chart()->addSeries(cathodeLoadLine);
    QObject::connect(cathodeLoadLine, &QLineSeries::hovered, designer, &Designer::on_seriesHovered);

    auto [operatingPoint, i, j] = intersection(anodeLoadLine, cathodeLoadLine);

    ia = operatingPoint.y();
    double va = operatingPoint.x();

    double vg = ia * rk / 1000;
    parameter[TRI_CF_IA]->setValue(ia);
    parameter[TRI_CF_VBIAS]->setValue(vg);
    parameter[TRI_CF_VK]->setValue(vb - operatingPoint.x());

    double ia1 = device->anodeCurrent(va, -(vg + 0.1));
    double deltaIa = (ia - ia1) / 1000.0;
    double gm = deltaIa / 0.1;

    parameter[TRI_CF_RO]->setValue(1.0 / gm);

    acLoadLine = new QLineSeries();
    acLoadLine->setPen(QPen(Qt::cyan));

    double rac = (ra + rk) * rl / (ra + rk + rl);
    double vac = va + rac * ia / 1000.0;
    double iac = vac / rac;

    acLoadLine->append(0, iac * 1000.0);
    acLoadLine->append(vac, 0.0);

    acLoadLine->setName("AC Load Line");
    chartView->chart()->addSeries(acLoadLine);
    QObject::connect(acLoadLine, &QLineSeries::hovered, designer, &Designer::on_seriesHovered);

    QScatterSeries *operatingPointSeries = new QScatterSeries();
    operatingPointSeries->setName("Operating Point");
    operatingPointSeries->setMarkerShape(QScatterSeries::MarkerShapeCircle);
    operatingPointSeries->setMarkerSize(10.0);
    operatingPointSeries->setColor(Qt::red);
    operatingPointSeries->append(operatingPoint);
    chartView->chart()->addSeries(operatingPointSeries);
    QObject::connect(operatingPointSeries, &QLineSeries::hovered, designer, &Designer::on_seriesHovered);
}

void ACCathodeFollower::showLoadLines(QChartView *chartView)
{
    anodeAxes(chartView);
}

void ACCathodeFollower::showDescription(QTextBrowser *textBrowser)
{
    textBrowser->setSource(QUrl("circuits/acCathodeFollower.html"));
}
