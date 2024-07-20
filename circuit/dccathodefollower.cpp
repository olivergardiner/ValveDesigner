#include "dccathodefollower.h"
#include "spice/spice.h"
#include "valvemodel/valvemodel.h"
#include "designer.h"

#include <QLineSeries>
#include <QScatterSeries>

DCCathodeFollower::DCCathodeFollower(Designer *designer)
    : Circuit(designer)
{
    parameter[nParameters++] = new Parameter("Supply Voltage:", 300.0, true);
    parameter[nParameters++] = new Parameter("Cathode Resistor (Rk):", 1500.0, true);
    parameter[nParameters++] = new Parameter("Anode Load (Ra):", 100000.0, true);
    parameter[nParameters++] = new Parameter("Follower Load (Rl):", 82000.0, true);
    parameter[nParameters++] = new Parameter("Bias Point (Vk):", 0.0, false);
    parameter[nParameters++] = new Parameter("Anode Voltage:", 0.0, false);
    parameter[nParameters++] = new Parameter("Anode Current:", 0.0, false);
    parameter[nParameters++] = new Parameter("Gain:", 0.0, false);
    parameter[nParameters++] = new Parameter("Follower Voltage:", 0.0, false);
    parameter[nParameters++] = new Parameter("Follower Current:", 0.0, false);
}

void DCCathodeFollower::showAnodeCharacteristics(QChartView *chartView)
{
    plotAnode(chartView);
}

void DCCathodeFollower::calculateOperatingPoint()
{
}

void DCCathodeFollower::calculateOperatingPoint(QChartView *chartView)
{
    double vgMax = device->getVg1Max();
    double mu = device->getModel()->getParameter(PAR_MU)->getValue();
    double vb = parameter[TRI_DC_CF_VB]->getValue();
    double rk = parameter[TRI_DC_CF_RK]->getValue();
    double rl = parameter[TRI_DC_CF_RL]->getValue();
    double ra = parameter[TRI_DC_CF_RA]->getValue();

    anodeLoadLine = new QLineSeries();
    anodeLoadLine->setPen(QPen(Qt::green));

    double ia = vb / (ra + rk);
    anodeLoadLine->append(0.0, ia * 1000.0);
    anodeLoadLine->append(vb, 0.0);

    anodeLoadLine->setName("Anode LL");
    chartView->chart()->addSeries(anodeLoadLine);
    QObject::connect(anodeLoadLine, &QLineSeries::hovered, designer, &Designer::on_seriesHovered);

    followerLoadLine = new QLineSeries();
    followerLoadLine->setPen(QPen(Qt::cyan));

    ia = vb / rl;
    followerLoadLine->append(0.0, ia * 1000.0);
    followerLoadLine->append(vb, 0.0);

    followerLoadLine->setName("Follower LL");
    chartView->chart()->addSeries(followerLoadLine);
    QObject::connect(followerLoadLine, &QLineSeries::hovered, designer, &Designer::on_seriesHovered);

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
    double vg = ia * rk / 1000;
    double va = operatingPoint.x();
    parameter[TRI_DC_CF_IA1]->setValue(ia);
    parameter[TRI_DC_CF_VK1]->setValue(vg);
    parameter[TRI_DC_CF_VA1]->setValue(operatingPoint.x());

    double ia1 = device->anodeCurrent(vb, -vg);
    double ia2 = device->anodeCurrent(vb - 10.0, -vg);

    double ar = 10000.0 / (ia1 - ia2);
    //parameter[TRI_CC_AR]->setValue(ar);

    double re = ra;

    parameter[TRI_DC_CF_GAIN]->setValue(mu * re / (re + ar));

    double vk2 = vb - operatingPoint.x();
    ia2 = 1000.0 * va / rl;
    parameter[TRI_DC_CF_VK2]->setValue(vk2);
    parameter[TRI_DC_CF_IA2]->setValue(ia2);

    QScatterSeries *operatingPointSeries = new QScatterSeries();
    operatingPointSeries->setName("Operating Point");
    operatingPointSeries->setMarkerShape(QScatterSeries::MarkerShapeCircle);
    operatingPointSeries->setMarkerSize(10.0);
    operatingPointSeries->setColor(Qt::red);
    operatingPointSeries->append(operatingPoint);
    operatingPointSeries->append(QPointF(vk2, ia2));
    chartView->chart()->addSeries(operatingPointSeries);
    QObject::connect(operatingPointSeries, &QLineSeries::hovered, designer, &Designer::on_seriesHovered);
}

void DCCathodeFollower::showLoadLines(QChartView *chartView)
{
    anodeAxes(chartView);
}

void DCCathodeFollower::showDescription(QTextBrowser *textBrowser)
{
    textBrowser->setSource(QUrl("circuits/dcCathodeFollower.html"));
}
