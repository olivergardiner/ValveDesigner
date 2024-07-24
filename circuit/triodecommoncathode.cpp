#include "triodecommoncathode.h"
#include "spice/spice.h"
#include "valvemodel/valvemodel.h"
#include "designer.h"

#include <QLineSeries>
#include <QScatterSeries>

TriodeCommonCathode::TriodeCommonCathode(Designer *designer)
    : Circuit(designer)
{
    parameter[nParameters++] = new Parameter("Supply Voltage:", 300.0, true);
    parameter[nParameters++] = new Parameter("Cathode Resistor (Rk):", 1000.0, true);
    parameter[nParameters++] = new Parameter("Anode Load (Ra):", 100000.0, true);
    parameter[nParameters++] = new Parameter("Circuit Load (Rl):", 1000000.0, true);
    parameter[nParameters++] = new Parameter("Bias Point (Vk):", 0.0, false);
    parameter[nParameters++] = new Parameter("Anode Voltage:", 0.0, false);
    parameter[nParameters++] = new Parameter("Anode Current:", 0.0, false);
    //parameter[nParameters++] = new Parameter("Anode Resistance:", 0.0, false);
    parameter[nParameters++] = new Parameter("Gain:", 0.0, false);
    parameter[nParameters++] = new Parameter("Gain (Rk bypassed):", 0.0, false);
}

void TriodeCommonCathode::showAnodeCharacteristics(QChartView *chartView)
{
    plotAnode(chartView);
}

void TriodeCommonCathode::calculateOperatingPoint()
{
    double vb = parameter[TRI_CC_VB]->getValue();
    double rk = parameter[TRI_CC_RK]->getValue();
    double ra = parameter[TRI_CC_RA]->getValue();
    double rl = parameter[TRI_CC_RL]->getValue();

    // First find the operating point using ngspice

    spice("source models/cc-triode.cir");

    device->alterParameters();

    spice("reset");

    QString command = QString {"alter vb=%1"}.arg(vb);
    spice(command.toStdString().c_str());

    command = QString {"alter rk=%1"}.arg(rk);
    spice(command.toStdString().c_str());

    command = QString {"alter ra=%1"}.arg(ra);
    spice(command.toStdString().c_str());

    spice("bg_run");

    if (!waitSimulationEnd()) {
        // The simulation did not complete inside 2s
        return;
    }

    double vgBias = 0.0;
    pvector_info myvec = ngGet_Vec_Info(const_cast<char *>("V(3)"));
    if (myvec && myvec->v_length > 0) {
        vgBias = myvec->v_realdata[0];
        parameter[TRI_CC_VK]->setValue(vgBias);
    }

    myvec = ngGet_Vec_Info(const_cast<char *>("V(4)"));
    if (myvec && myvec->v_length > 0) {
        parameter[TRI_CC_VA]->setValue(myvec->v_realdata[0]);
    }

    myvec = ngGet_Vec_Info(const_cast<char *>("I(VM)"));
    if (myvec && myvec->v_length > 0) {
        parameter[TRI_CC_IA]->setValue(myvec->v_realdata[0] * 1000.0);
    }

    double ia1 = device->anodeCurrent(vb, -vgBias);
    double ia2 = device->anodeCurrent(vb - 10.0, -vgBias);

    double ar = 10000.0 / (ia1 - ia2);
    //parameter[TRI_CC_AR]->setValue(ar);

    double mu = device->getModel()->getParameter(PAR_MU)->getValue();

    double re = ra * rl / (ra + rl);

    double ark = ar + rk * (mu + 1.0);
    parameter[TRI_CC_GAIN]->setValue(mu * re / (re + ark));
    parameter[TRI_CC_GAIN_B]->setValue(mu * re / (re + ar));
}

void TriodeCommonCathode::calculateOperatingPoint(QChartView *chartView)
{
    double vgMax = device->getVg1Max();
    double mu = device->getModel()->getParameter(PAR_MU)->getValue();
    double vb = parameter[TRI_CC_VB]->getValue();
    double rk = parameter[TRI_CC_RK]->getValue();
    double rl = parameter[TRI_CC_RL]->getValue();
    double ra = parameter[TRI_CC_RA]->getValue();

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
    double vg = ia * rk / 1000;
    parameter[TRI_CC_IA]->setValue(ia);
    parameter[TRI_CC_VK]->setValue(vg);
    parameter[TRI_CC_VA]->setValue(operatingPoint.x());

    double ia1 = device->anodeCurrent(vb, -vg);
    double ia2 = device->anodeCurrent(vb - 10.0, -vg);

    double ar = 10000.0 / (ia1 - ia2);
    //parameter[TRI_CC_AR]->setValue(ar);

    double re = ra * rl / (ra + rl);
    double ark = ar + rk * (mu + 1.0);
    parameter[TRI_CC_GAIN]->setValue(mu * re / (re + ark));
    parameter[TRI_CC_GAIN_B]->setValue(mu * re / (re + ar));

    QScatterSeries *operatingPointSeries = new QScatterSeries();
    operatingPointSeries->setName("Operating Point");
    operatingPointSeries->setMarkerShape(QScatterSeries::MarkerShapeCircle);
    operatingPointSeries->setMarkerSize(10.0);
    operatingPointSeries->setColor(Qt::red);
    operatingPointSeries->append(operatingPoint);
    chartView->chart()->addSeries(operatingPointSeries);
    QObject::connect(operatingPointSeries, &QLineSeries::hovered, designer, &Designer::on_seriesHovered);
}

void TriodeCommonCathode::showLoadLines(QChartView *chartView)
{
    anodeAxes(chartView);
}

void TriodeCommonCathode::showDescription(QTextBrowser *textBrowser)
{
    textBrowser->setSource(QUrl("circuits/triodeCommonCathode.html"));
}
