#pragma once

#include "cohenhelietriode.h"

class GardinerPentode : public CohenHelieTriode
{
public:
    GardinerPentode();

    double anodeCurrent(double va, double vg1, double vg2, bool secondaryEmission);
    double screenCurrent(double va, double vg1, double vg2, bool secondaryEmission);
    void _fromJson(QJsonObject source);
    void toJson(QJsonObject &destination);
    QString getName() const;
    int getDeviceType() const;
    void alterParameters();
};
