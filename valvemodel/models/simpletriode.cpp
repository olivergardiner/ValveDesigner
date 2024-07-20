#include "simpletriode.h"
#include "spice/spice.h"
#include "valvemodel/valvemodel.h"

SimpleTriode::SimpleTriode() {}

double SimpleTriode::anodeCurrent(double va, double vg1, double vg2, bool secondaryEmission)
{
    double ia = 0.0;

    double e1t = va / parameter[PAR_MU]->getValue() + vg1 + parameter[PAR_VCT]->getValue();
    if (e1t > 0) {
        ia = pow(e1t, parameter[PAR_X]->getValue()) / parameter[PAR_KG1]->getValue();
    }

    return ia;
}

void SimpleTriode::_fromJson(QJsonObject source)
{
    if (source.contains("kg1") && source["kg1"].isDouble()) {
        parameter[PAR_KG1]->setValue(source["kg1"].toDouble() / 1000.0);
    }

    if (source.contains("mu") && source["mu"].isDouble()) {
        parameter[PAR_MU]->setValue(source["mu"].toDouble());
    }

    if (source.contains("x") && source["x"].isDouble()) {
        parameter[PAR_X]->setValue(source["x"].toDouble());
    }

    if (source.contains("vct") && source["vct"].isDouble()) {
        parameter[PAR_VCT]->setValue(source["vct"].toDouble());
    }
}

void SimpleTriode::toJson(QJsonObject &destination)
{
    destination["kg1"] = parameter[PAR_KG1]->getValue() * 1000.0;
    destination["mu"] = parameter[PAR_MU]->getValue();
    destination["x"] = parameter[PAR_X]->getValue();
    destination["vct"] = parameter[PAR_VCT]->getValue();

    destination["device"] = "triode";
    destination["type"] = "simple";
}

QString SimpleTriode::getName() const
{
    return "Simple Triode";
}

int SimpleTriode::getDeviceType() const
{
    return TRIODE;
}

void SimpleTriode::alterParameters()
{
    QString command = QString {"alterparam mu=%1"}.arg(getParameter(PAR_MU)->getValue());
    spice(command.toStdString().c_str());

    command = QString {"alterparam kg1=%1"}.arg(getParameter(PAR_KG1)->getValue() * 1000.0);
    spice(command.toStdString().c_str());

    command = QString {"alterparam x=%1"}.arg(getParameter(PAR_X)->getValue());
    spice(command.toStdString().c_str());

    command = QString {"alterparam vct=%1"}.arg(getParameter(PAR_VCT)->getValue());
    spice(command.toStdString().c_str());
}
