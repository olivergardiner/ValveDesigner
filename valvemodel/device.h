#pragma once

#include "model.h"

#include <QString>

/**
 * @brief The Device class wraps a model with other device specific information needed to be able to simulate the device.
 *
 */
class Device
{
public:
    Device();
    Device(QJsonObject source);

    double anodeCurrent(double va, double vg1, double vg2 = 0);
    double anodeVoltage(double ia, double vg1, double vg2 = 0);
    double screenCurrent(double va, double vg1, double vg2);
    double screenVoltage(double ik, double va, double vg1);
    double cathodeCurrent(double va, double vg1, double vg2);

    Parameter *getParameter(int parameterIndex);

    void fromJson(QJsonObject source);
    void toJson(QJsonObject &destination);

    Model *getModel() const;
    void setModel(Model *newModel);
    QString getName() const;
    void setName(const QString &newName);
    double getVaMax() const;
    void setVaMax(double newVaMax);
    double getIaMax() const;
    void setIaMax(double newIaMax);
    double getVg1Max() const;
    void setVg1Max(double newVg1Max);
    double getVg2Max() const;
    void setVg2Max(double newVg2Max);
    double getIg2Max() const;
    void setIg2Max(double newIg2Max);
    double getPaMax() const;
    void setPaMax(double newPaMax);
    double getPg2Max() const;
    void setPg2Max(double newPg2Max);

    void alterParameters();

private:
    Model *model = nullptr;

    QString name;

    double vaMax = 0.0;
    double iaMax = 0.0;
    double vg1Max = 0.0;
    double vg2Max = 0.0;
    double ig2Max = 0.0;
    double paMax = 0.0;
    double pg2Max = 0.0;
};
