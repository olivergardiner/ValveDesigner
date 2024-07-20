#include "gardinerpentode.h"
#include "spice/spice.h"
#include "valvemodel/valvemodel.h"

GardinerPentode::GardinerPentode() {}

double GardinerPentode::anodeCurrent(double va, double vg1, double vg2, bool secondaryEmission)
{
    double epk = cohenHelieEpk(vg2, vg1);
    double k = 1.0 / parameter[PAR_KG1]->getValue() - 1.0 / parameter[PAR_KG2]->getValue();
    double shift = parameter[PAR_BETA]->getValue() * (1.0 - parameter[PAR_ALPHA]->getValue() * vg1);
    double g = exp(-pow(shift * va, parameter[PAR_GAMMA]->getValue()));
    double scale = 1.0 - g;
    double vco = vg2 / parameter[PAR_LAMBDA]->getValue() - vg1 * parameter[PAR_NU]->getValue() - parameter[PAR_OMEGA]->getValue();
    double psec = parameter[PAR_S]->getValue() * va * (1.0 + tanh(-parameter[PAR_AP]->getValue() * (va - vco)));
    double ia = epk * (k * scale + parameter[PAR_A]->getValue() * va / parameter[PAR_KG2]->getValue()) + parameter[PAR_OS]->getValue() * vg2;

    if(secondaryEmission) {
        ia = ia - epk * psec / parameter[PAR_KG2]->getValue();
    }

    return ia;
}

double GardinerPentode::screenCurrent(double va, double vg1, double vg2, bool secondaryEmission)
{
    double epk = cohenHelieEpk(vg2, vg1);
    double shift = parameter[PAR_RHO]->getValue() * (1.0 - parameter[PAR_TAU]->getValue() * vg1);
    double h = exp(-pow(shift * va, parameter[PAR_THETA]->getValue() * 0.9));
    double vco = vg2 / parameter[PAR_LAMBDA]->getValue() - vg1 * parameter[PAR_NU]->getValue() - parameter[PAR_OMEGA]->getValue();
    double psec = parameter[PAR_S]->getValue() * va * (1.0 + tanh(-parameter[PAR_AP]->getValue() * (va - vco)));
    double ig2 = epk * (1.0 + parameter[PAR_PSI]->getValue() * h) / parameter[PAR_KG2A]->getValue() - epk * parameter[PAR_A]->getValue() * va / parameter[PAR_KG2A]->getValue();

    if(secondaryEmission) {
        ig2 = ig2 + epk * psec / parameter[PAR_KG2A]->getValue();
    }

    return ig2;
}

void GardinerPentode::_fromJson(QJsonObject source)
{
    CohenHelieTriode::_fromJson(source);

    if (source.contains("kg2") && source["kg2"].isDouble()) {
        parameter[PAR_KG2]->setValue(source["kg2"].toDouble() / 1000.0);
    }

    if (source.contains("a") && source["a"].isDouble()) {
        parameter[PAR_A]->setValue(source["a"].toDouble());
    }

    if (source.contains("alpha") && source["alpha"].isDouble()) {
        parameter[PAR_ALPHA]->setValue(source["alpha"].toDouble());
    }

    if (source.contains("beta") && source["beta"].isDouble()) {
        parameter[PAR_BETA]->setValue(source["beta"].toDouble());
    }

    if (source.contains("gamma") && source["gamma"].isDouble()) {
        parameter[PAR_GAMMA]->setValue(source["gamma"].toDouble());
    }

    if (source.contains("kg2a") && source["kg2a"].isDouble()) {
        parameter[PAR_KG2A]->setValue(source["kg2a"].toDouble() / 1000.0);
    }

    if (source.contains("tau") && source["tau"].isDouble()) {
        parameter[PAR_TAU]->setValue(source["tau"].toDouble());
    }

    if (source.contains("rho") && source["rho"].isDouble()) {
        parameter[PAR_RHO]->setValue(source["rho"].toDouble());
    }

    if (source.contains("theta") && source["theta"].isDouble()) {
        parameter[PAR_THETA]->setValue(source["theta"].toDouble());
    }

    if (source.contains("psi") && source["psi"].isDouble()) {
        parameter[PAR_PSI]->setValue(source["psi"].toDouble());
    }

    if (source.contains("omega") && source["omega"].isDouble()) {
        parameter[PAR_OMEGA]->setValue(source["omega"].toDouble());
    }

    if (source.contains("lambda") && source["lambda"].isDouble()) {
        parameter[PAR_LAMBDA]->setValue(source["lambda"].toDouble());
    }

    if (source.contains("nu") && source["nu"].isDouble()) {
        parameter[PAR_NU]->setValue(source["nu"].toDouble());
    }

    if (source.contains("s") && source["s"].isDouble()) {
        parameter[PAR_S]->setValue(source["s"].toDouble());
    }

    if (source.contains("ap") && source["ap"].isDouble()) {
        parameter[PAR_AP]->setValue(source["ap"].toDouble());
    }

    if (source.contains("os") && source["os"].isDouble()) {
        parameter[PAR_OS]->setValue(source["os"].toDouble());
    }
}

void GardinerPentode::toJson(QJsonObject &destination)
{
    CohenHelieTriode::toJson(destination);

    destination["kg2"] = parameter[PAR_KG2]->getValue() * 1000.0;
    destination["a"] = parameter[PAR_A]->getValue();
    destination["alpha"] = parameter[PAR_ALPHA]->getValue();
    destination["beta"] = parameter[PAR_BETA]->getValue();
    destination["gamma"] = parameter[PAR_GAMMA]->getValue();

    destination["kg2a"] = parameter[PAR_KG2A]->getValue() * 1000.0;
    destination["tau"] = parameter[PAR_TAU]->getValue();
    destination["rho"] = parameter[PAR_RHO]->getValue();
    destination["theta"] = parameter[PAR_THETA]->getValue();
    destination["psi"] = parameter[PAR_PSI]->getValue();

    destination["omega"] = parameter[PAR_OMEGA]->getValue();
    destination["lambda"] = parameter[PAR_LAMBDA]->getValue();
    destination["nu"] = parameter[PAR_NU]->getValue();
    destination["s"] = parameter[PAR_S]->getValue();
    destination["ap"] = parameter[PAR_AP]->getValue();

    destination["os"] = parameter[PAR_OS]->getValue();

    destination["device"] = "pentode";
    destination["type"] = "gardiner";
}

QString GardinerPentode::getName() const
{
    return "Gardiner Pentode";
}

int GardinerPentode::getDeviceType() const
{
    return PENTODE;
}

void GardinerPentode::alterParameters()
{
    CohenHelieTriode::alterParameters();

    QString command = QString {"alterparam kg2=%1"}.arg(getParameter(PAR_KG2)->getValue() * 1000.0);
    spice(command.toStdString().c_str());

    command = QString {"alterparam a=%1"}.arg(getParameter(PAR_A)->getValue());
    spice(command.toStdString().c_str());

    command = QString {"alterparam alpha=%1"}.arg(getParameter(PAR_ALPHA)->getValue());
    spice(command.toStdString().c_str());

    command = QString {"alterparam beta=%1"}.arg(getParameter(PAR_BETA)->getValue());
    spice(command.toStdString().c_str());

    command = QString {"alterparam gamma=%1"}.arg(getParameter(PAR_GAMMA)->getValue());
    spice(command.toStdString().c_str());

    command = QString {"alterparam kg2a=%1"}.arg(getParameter(PAR_KG2A)->getValue() * 1000.0);
    spice(command.toStdString().c_str());

    command = QString {"alterparam tau=%1"}.arg(getParameter(PAR_TAU)->getValue());
    spice(command.toStdString().c_str());

    command = QString {"alterparam rho=%1"}.arg(getParameter(PAR_RHO)->getValue());
    spice(command.toStdString().c_str());

    command = QString {"alterparam theta=%1"}.arg(getParameter(PAR_THETA)->getValue());
    spice(command.toStdString().c_str());

    command = QString {"alterparam psi=%1"}.arg(getParameter(PAR_PSI)->getValue());
    spice(command.toStdString().c_str());

    command = QString {"alterparam omega=%1"}.arg(getParameter(PAR_OMEGA)->getValue());
    spice(command.toStdString().c_str());

    command = QString {"alterparam lambda=%1"}.arg(getParameter(PAR_LAMBDA)->getValue());
    spice(command.toStdString().c_str());

    command = QString {"alterparam nu=%1"}.arg(getParameter(PAR_NU)->getValue());
    spice(command.toStdString().c_str());

    command = QString {"alterparam s=%1"}.arg(getParameter(PAR_S)->getValue());
    spice(command.toStdString().c_str());

    command = QString {"alterparam ap=%1"}.arg(getParameter(PAR_AP)->getValue());
    spice(command.toStdString().c_str());

    command = QString {"alterparam os=%1"}.arg(getParameter(PAR_OS)->getValue());
    spice(command.toStdString().c_str());
}
