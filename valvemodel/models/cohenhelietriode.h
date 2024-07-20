#pragma once

#include "korentriode.h"

class CohenHelieTriode : public KorenTriode
{
public:
    CohenHelieTriode();

    double anodeCurrent(double va, double vg1, double vg2, bool secondaryEmission);
    void _fromJson(QJsonObject source);
    void toJson(QJsonObject &destination);
    QString getName() const;
    int getDeviceType() const;
    void alterParameters();

protected:
    double cohenHelieEpk(double v, double vg);
    static double cohenHelieEpk(double v, double vg, double kp, double kvb, double kvb2, double vct, double a, double mu);
    static double cohenHelieCurrent(double va, double vg, double kg1, double kp, double kvb, double kvb2, double vct, double a, double mu);
};
