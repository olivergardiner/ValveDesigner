#include "korentriode.h"
#include "spice/spice.h"
#include "valvemodel/valvemodel.h"

KorenTriode::KorenTriode() {}

double KorenTriode::anodeCurrent(double va, double vg1, double vg2, bool secondaryEmission)
{
    return korenCurrent(va, vg1,
                        parameter[PAR_KP]->getValue(),
                        parameter[PAR_KVB]->getValue(),
                        parameter[PAR_X]->getValue(),
                        parameter[PAR_MU]->getValue()) / parameter[PAR_KG1]->getValue();

}

void KorenTriode::_fromJson(QJsonObject source)
{
    SimpleTriode::_fromJson(source);

    if (source.contains("kp") && source["kp"].isDouble()) {
        parameter[PAR_KP]->setValue(source["kp"].toDouble());
    }

    if (source.contains("kvb") && source["kvb"].isDouble()) {
        parameter[PAR_KVB]->setValue(source["kvb"].toDouble());
    }
}

void KorenTriode::toJson(QJsonObject &destination)
{
    SimpleTriode::toJson(destination);

    destination["kp"] = parameter[PAR_KP]->getValue();
    destination["kvb"] = parameter[PAR_KVB]->getValue();

    destination["device"] = "triode";
    destination["type"] = "koren";
}

QString KorenTriode::getName() const
{
    return "Koren Triode";
}

int KorenTriode::getDeviceType() const
{
    return TRIODE;
}

void KorenTriode::alterParameters()
{
    SimpleTriode::alterParameters();

    QString command = QString {"alterparam kp=%1"}.arg(getParameter(PAR_KP)->getValue());
    spice(command.toStdString().c_str());

    command = QString {"alterparam kvb=%1"}.arg(getParameter(PAR_KVB)->getValue());
    spice(command.toStdString().c_str());
}

double KorenTriode::korenCurrent(double va, double vg, double kp, double kvb, double a, double mu)
{
    double x1 = std::sqrt(kvb + va * va);
    double x2 = kp * (1 / mu + vg / x1);
    double x3 = std::log(1.0 + std::exp(x2));
    double et = (va / kp) * x3;

    if (et < 0.0) {
        et = 0.0;
    }

    return pow(et, a);
}
