#include "reefmanpentode.h"
#include "valvemodel/valvemodel.h"

ReefmanPentode::ReefmanPentode() {}

double ReefmanPentode::anodeCurrent(double va, double vg1, double vg2, bool secondaryEmission)
{
    return 0.0;
}

double ReefmanPentode::screenCurrent(double va, double vg1, double vg2, bool secondaryEmission)
{
    return 0.0;
}

void ReefmanPentode::_fromJson(QJsonObject source)
{

}

void ReefmanPentode::toJson(QJsonObject &destination)
{

}

QString ReefmanPentode::getName() const
{
    return "Reefman Pentode";
}

int ReefmanPentode::getDeviceType() const
{
    return PENTODE;
}

void ReefmanPentode::alterParameters()
{

}
