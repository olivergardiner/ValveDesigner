#pragma once

#include <QJsonObject>

#include "parameter.h"

class Model
{
public:
    Model();

    virtual double anodeCurrent(double va, double vg1, double vg2 = 0.0, bool secondaryEmission = true) = 0;
    virtual double screenCurrent(double va, double vg1, double vg2, bool secondaryEmission = true);

    double cathodeCurrent(double va, double vg1, double vg2 = 0.0, bool secondaryEmission = true);
    double anodeVoltage(double ia, double vg1, double vg2 = 0.0, bool secondaryEmission = true);
    double screenVoltage(double ik, double va, double vg1, bool secondaryEmission = true);

    double getParameterValue(int parameterIndex);
    Parameter *getParameter(int parameterIndex);

    static Model *fromJson(QJsonObject source);
    virtual void toJson(QJsonObject &destination) = 0;

    virtual QString getName() const = 0;
    virtual int getDeviceType() const = 0;

    virtual void alterParameters() = 0;

protected:
    virtual void _fromJson(QJsonObject source) = 0;

    /**
     * @brief parameter The array of named model parameters
     */
    Parameter *parameter[24];
};
