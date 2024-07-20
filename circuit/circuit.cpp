#include "circuit.h"
#include "designer.h"

#include <QDoubleValidator>
#include <QLineSeries>
#include <QValueAxis>

Circuit::Circuit(Designer *designer)
{
    this->designer = designer;
    nParameters = 0;
}

void Circuit::updateParameters(QLabel *labels[], QLineEdit *values[])
{
    for (int i = 0; i < 16; i++) {
        if (i < nParameters) {
            parameter[i]->updateUI(labels[i], values[i]);
        } else {
            labels[i]->setVisible(false);
            values[i]->setVisible(false);
        }
    }
}

void Circuit::parameterChanged(int index, QString value)
{
    QDoubleValidator validator;
    int pos;
    if (validator.validate(value, pos) != QValidator::Invalid) {
        parameter[index]->setValue(value.toDouble());
    }
}

void Circuit::setDevice(Device *newDevice)
{
    device = newDevice;
}

double Circuit::interval(double maxValue)
{
    double interval = 0.5;

    if (maxValue > 5.0) {
        interval = 1.0;
    }
    if (maxValue > 10.0) {
        interval = 2.0;
    }
    if (maxValue > 20.0) {
        interval = 5.0;
    }
    if (maxValue > 50.0) {
        interval = 10.0;
    }
    if (maxValue > 100.0) {
        interval = 20.0;
    }
    if (maxValue > 200.0) {
        interval = 50.0;
    }
    if (maxValue > 500.0) {
        interval = 100.0;
    }

    return interval;
}

int Circuit::tickCount(double maxValue)
{
    return (maxValue + 0.01) / interval(maxValue) + 1;
}

void Circuit::plotAnode(QChartView *chartView)
{
    double vg1Step = interval(device->getVg1Max());
    double vg1 = 0.0;
    while (vg1 < device->getVg1Max() + 0.001) {
        QLineSeries *anode = new QLineSeries();
        anode->setName(QString("Vg1 = %1").arg(-vg1));
        for (int i = 1; i < 101; i++) {
            double va = device->getVaMax() * i / 100.0;
            anode->append(va, device->anodeCurrent(va, -vg1));
        }
        anode->setPen(QPen(Qt::blue));
        chartView->chart()->addSeries(anode);
        QObject::connect(anode, &QLineSeries::hovered, designer, &Designer::on_seriesHovered);
        vg1 += vg1Step;
    }

    plotAnodePower(chartView);

    anodeAxes(chartView);
}

void Circuit::plotAnode(QChartView *chartView, double vg2)
{
    plotScreen(chartView, vg2);

    double vg1Step = interval(device->getVg1Max());
    double vg1 = 0.0;
    while (vg1 < device->getVg1Max() + 0.001) {
        QLineSeries *anode = new QLineSeries();
        anode->setName(QString("Vg1 = %1").arg(-vg1));
        for (int i = 0; i < 101; i++) {
            double va = device->getVaMax() * i / 100.0;
            anode->append(va, device->anodeCurrent(va, -vg1, vg2));
        }
        anode->setPen(QPen(Qt::blue));
        chartView->chart()->addSeries(anode);
        QObject::connect(anode, &QLineSeries::hovered, designer, &Designer::on_seriesHovered);
        vg1 += vg1Step;
    }

    plotAnodePower(chartView);

    anodeAxes(chartView);
}

void Circuit::plotAnode(QChartView *chartView, double vht, double ulTap)
{
    double vg1Step = interval(device->getVg1Max());
    double vg1 = 0.0;
    while (vg1 < device->getVg1Max() + 0.001) {
        QLineSeries *anode = new QLineSeries();
        anode->setName(QString("Vg1 = %1").arg(-vg1));
        for (int i = 0; i < 101; i++) {
            double va = device->getVaMax() * i / 100.0;
            double vg2 = vht - ulTap * (vht - va);
            anode->append(va, device->anodeCurrent(va, -vg1, vg2));
        }
        anode->setPen(QPen(Qt::blue));
        chartView->chart()->addSeries(anode);
        QObject::connect(anode, &QLineSeries::hovered, designer, &Designer::on_seriesHovered);
        vg1 += vg1Step;
    }

    plotAnodePower(chartView);

    anodeAxes(chartView);
}

void Circuit::plotScreen(QChartView *chartView, double vg2)
{
    double vg1Step = interval(device->getVg1Max());
    double vg1 = 0.0;
    while (vg1 < device->getVg1Max() + 0.001) {
        QLineSeries *screen = new QLineSeries();
        screen->setName(QString("Vg1 = %1").arg(-vg1));
        for (int i = 1; i < 101; i++) {
            double va = device->getVaMax() * i / 100.0;
            screen->append(va, device->screenCurrent(va, -vg1, vg2));
        }
        screen->setPen(QPen(Qt::cyan));
        chartView->chart()->addSeries(screen);
        QObject::connect(screen, &QLineSeries::hovered, designer, &Designer::on_seriesHovered);
        vg1 += vg1Step;
    }
}

void Circuit::plotAnodePower(QChartView *chartView)
{
    if (device->getPaMax() > 0.0) {
        anodePowerLimit = new QLineSeries();
        anodePowerLimit->setPen(QPen(Qt::red));
        anodePowerLimit->setName(QString("Pa = %1W").arg(device->getPaMax()));
        double startVa = 1000 * device->getPaMax() / device->getIaMax();
        for (int i=0; i < 100; i++) {
            double va = startVa + (device->getVaMax() - startVa) * i / 100.0;
            anodePowerLimit->append(va, 1000 * device->getPaMax() / va);
        }
        chartView->chart()->addSeries(anodePowerLimit);
        QObject::connect(anodePowerLimit, &QLineSeries::hovered, designer, &Designer::on_seriesHovered);
    }
}

void Circuit::plotScreenPower(QChartView *chartView)
{
}

void Circuit::anodeAxes(QChartView *chartView)
{
    QChart *chart = chartView->chart();

    chart->createDefaultAxes();

    auto xAxis = chart->axes(Qt::Horizontal).back();
    if (xAxis->type() == QAbstractAxis::AxisTypeValue)
        static_cast<QValueAxis*>(xAxis)->setTickCount(tickCount(device->getVaMax()));
    auto yAxis = chart->axes(Qt::Vertical).back();
    if (yAxis->type() == QAbstractAxis::AxisTypeValue)
        static_cast<QValueAxis*>(yAxis)->setTickCount(tickCount(device->getIaMax()));
    xAxis->setRange(0.0, device->getVaMax());
    xAxis->setTitleText("Va (V)");
    yAxis->setRange(0.0, device->getIaMax());
    yAxis->setTitleText("Ia (mA)");
    chart->setTitle(device->getName());
    chart->legend()->hide();
    chartView->setRenderHints(QPainter::Antialiasing);
}

std::tuple<QPointF, int, int> Circuit::intersection(QLineSeries *series1, QLineSeries *series2)
{
    for (int i = 0; i < series1->count() - 1; i++) {
        QPointF p1 = series1->at(i);
        QPointF p2 = series1->at(i + 1);
        for (int j = 0; j < series2->count() - 1; j++) {
            QPointF p3 = series2->at(j);
            QPointF p4 = series2->at(j + 1);
            double x1 = p1.x();
            double y1 = p1.y();
            double x2 = p2.x();
            double y2 = p2.y();
            double x3 = p3.x();
            double y3 = p3.y();
            double x4 = p4.x();
            double y4 = p4.y();
            double a1 = y2 - y1;
            double b1 = x1 - x2;
            double c1 = a1 * x1 + b1 * y1;
            double a2 = y4 - y3;
            double b2 = x3 - x4;
            double c2 = a2 * x3 + b2 * y3;
            double d = a1 * b2 - a2 * b1;
            if (d != 0.0) {
                double x = (b2 * c1 - b1 * c2) / d;
                double y = (a1 * c2 - a2 * c1) / d;

                bool onLine1 = x > qMin(x1, x2) && x < qMax(x1, x2) && y > qMin(y1, y2) && y < qMax(y1, y2);
                bool onLine2 = x > qMin(x3, x4) && x < qMax(x3, x4) && y > qMin(y3, y4) && y < qMax(y3, y4);

                if (onLine1 && onLine2) {
                    return {QPointF(x, y), i , j};
                }
            }
        }
    }

    return {QPointF(0, 0), -1, -1};
}
