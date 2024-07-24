#include "cohenhelietriode.h"
#include "spice/spice.h"
#include "valvemodel/valvemodel.h"

CohenHelieTriode::CohenHelieTriode() {}

double CohenHelieTriode::anodeCurrent(double va, double vg1, double vg2, bool secondaryEmission)
{
    return cohenHelieCurrent(va, vg1,
                             parameter[PAR_KG1]->getValue(),
                             parameter[PAR_KP]->getValue(),
                             parameter[PAR_KVB]->getValue(),
                             parameter[PAR_KVB1]->getValue(),
                             parameter[PAR_VCT]->getValue(),
                             parameter[PAR_X]->getValue(),
                             parameter[PAR_MU]->getValue());
}

void CohenHelieTriode::_fromJson(QJsonObject source)
{
    KorenTriode::_fromJson(source);

    if (source.contains("kvb1") && source["kvb1"].isDouble()) {
        parameter[PAR_KVB1]->setValue(source["kvb1"].toDouble());
    }
}

void CohenHelieTriode::toJson(QJsonObject &model)
{
    KorenTriode::toJson(model);

    model["kvb1"] = parameter[PAR_KVB1]->getValue();

    model["device"] = "triode";
    model["type"] = "cohenHelie";
}

QString CohenHelieTriode::getName() const
{
    return "Cohen Helie Triode";
}

int CohenHelieTriode::getDeviceType() const
{
    return TRIODE;
}

double CohenHelieTriode::cohenHelieEpk(double v, double vg)
{
    double kp = parameter[PAR_KP]->getValue();
    double kvb = parameter[PAR_KVB]->getValue();
    double kvb1 = parameter[PAR_KVB1]->getValue();
    double vct = parameter[PAR_VCT]->getValue();
    double x = parameter[PAR_X]->getValue();
    double mu = parameter[PAR_MU]->getValue();

    return cohenHelieEpk(v, vg, kp, kvb, kvb1, vct, x, mu);
}

double CohenHelieTriode::cohenHelieEpk(double v, double vg, double kp, double kvb, double kvb1, double vct, double x, double mu)
{
    double f = std::sqrt(kvb + v * kvb1 + v * v);
    double y = kp * (1 / mu + (vg + vct) / f);
    double ep = (v / kp) * std::log(1.0 + std::exp(y));

    return pow(ep, x);
}

double CohenHelieTriode::cohenHelieCurrent(double v, double vg, double kg1, double kp, double kvb, double kvb1, double vct, double x, double mu)
{
    return cohenHelieEpk(v, vg, kp, kvb, kvb1, vct, x, mu) / kg1;
}

void CohenHelieTriode::alterParameters()
{
    KorenTriode::alterParameters();

    QString command = QString {"alterparam kvb1=%1"}.arg(getParameter(PAR_KVB1)->getValue());
    spice(command.toStdString().c_str());
}


