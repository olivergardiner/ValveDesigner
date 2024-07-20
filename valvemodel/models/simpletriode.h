#pragma once

#include "../model.h"

class SimpleTriode : public Model
{
public:
    SimpleTriode();

    double anodeCurrent(double va, double vg1, double vg2, bool secondaryEmission);
    void _fromJson(QJsonObject source);
    void toJson(QJsonObject &destination);
    QString getName() const;
    int getDeviceType() const;
    void alterParameters();
};
