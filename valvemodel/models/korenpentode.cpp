#include "korenpentode.h"
#include "valvemodel/valvemodel.h"

KorenPentode::KorenPentode() {}

double KorenPentode::anodeCurrent(double va, double vg1, double vg2, bool secondaryEmission)
{
    return 0.0;
}

double KorenPentode::screenCurrent(double va, double vg1, double vg2, bool secondaryEmission)
{
    return 0.0;
}

void KorenPentode::_fromJson(QJsonObject source)
{

}

void KorenPentode::toJson(QJsonObject &destination)
{

}

QString KorenPentode::getName() const
{
    return "Koren Pentode";
}

int KorenPentode::getDeviceType() const
{
    return PENTODE;
}

void KorenPentode::alterParameters()
{

}

