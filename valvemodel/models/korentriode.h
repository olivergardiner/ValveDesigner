#pragma once

#include "simpletriode.h"

class KorenTriode : public SimpleTriode
{
public:
    KorenTriode();

    double anodeCurrent(double va, double vg1, double vg2, bool secondaryEmission);
    void _fromJson(QJsonObject source);
    void toJson(QJsonObject &destination);
    QString getName() const;
    int getDeviceType() const;
    void alterParameters();

protected:
    double korenCurrent(double va, double vg, double kp, double kvb, double a, double mu);
};
