#include "device.h"

Device::Device() {}

Device::Device(QJsonObject source)
{
    fromJson(source);
}

double Device::anodeCurrent(double va, double vg1, double vg2)
{
    if (model != nullptr) {
        return model->anodeCurrent(va, vg1, vg2);
    }

    return 0.0;
}

double Device::anodeVoltage(double ia, double vg1, double vg2)
{
    if (model != nullptr) {
        return model->anodeVoltage(ia, vg1, vg2);
    }

    return 0.0;
}

double Device::screenCurrent(double va, double vg1, double vg2)
{
    if (model != nullptr) {
        return model->screenCurrent(va, vg1, vg2);
    }

    return 0.0;
}

double Device::screenVoltage(double ik, double va, double vg1)
{
    if (model != nullptr) {
        return model->screenVoltage(ik, va, vg1);
    }

    return 0.0;
}

double Device::cathodeCurrent(double va, double vg1, double vg2)
{
    if (model != nullptr) {
        return model->cathodeCurrent(va, vg1, vg2);
    }

    return 0.0;
}

Parameter *Device::getParameter(int parameterIndex)
{
    return model->getParameter(parameterIndex);
}

void Device::fromJson(QJsonObject source)
{
    if (source.contains("name") && source["name"].isString()) {
        name = source["name"].toString();
    }

    if (source.contains("vaMax") && source["vaMax"].isDouble()) {
        vaMax = source["vaMax"].toDouble();
    }

    if (source.contains("vg1Max") && source["vg1Max"].isDouble()) {
        vg1Max = source["vg1Max"].toDouble();
    }

    if (source.contains("vg2Max") && source["vg2Max"].isDouble()) {
        vg2Max = source["vg2Max"].toDouble();
    }

    if (source.contains("iaMax") && source["iaMax"].isDouble()) {
        iaMax = source["iaMax"].toDouble();
    }

    if (source.contains("paMax") && source["paMax"].isDouble()) {
        paMax = source["paMax"].toDouble();
    }

    if (source.contains("pg2Max") && source["pg2Max"].isDouble()) {
        pg2Max = source["pg2Max"].toDouble();
    }

    if (source.contains("model") && source["model"].isObject()) {
        QJsonObject modelObject = source["model"].toObject();

        model = Model::fromJson(modelObject);
    }
}

void Device::toJson(QJsonObject &destination)
{

}

Model *Device::getModel() const
{
    return model;
}

void Device::setModel(Model *newModel)
{
    model = newModel;
}

QString Device::getName() const
{
    return name;
}

void Device::setName(const QString &newName)
{
    name = newName;
}

double Device::getVaMax() const
{
    return vaMax;
}

void Device::setVaMax(double newVaMax)
{
    vaMax = newVaMax;
}

double Device::getIaMax() const
{
    return iaMax;
}

void Device::setIaMax(double newIaMax)
{
    iaMax = newIaMax;
}

double Device::getVg1Max() const
{
    return vg1Max;
}

void Device::setVg1Max(double newVg1Max)
{
    vg1Max = newVg1Max;
}

double Device::getVg2Max() const
{
    return vg2Max;
}

void Device::setVg2Max(double newVg2Max)
{
    vg2Max = newVg2Max;
}

double Device::getIg2Max() const
{
    return ig2Max;
}

void Device::setIg2Max(double newIg2Max)
{
    ig2Max = newIg2Max;
}

double Device::getPaMax() const
{
    return paMax;
}

void Device::setPaMax(double newPaMax)
{
    paMax = newPaMax;
}

double Device::getPg2Max() const
{
    return pg2Max;
}

void Device::setPg2Max(double newPg2Max)
{
    pg2Max = newPg2Max;
}

void Device::alterParameters()
{
    if (model != nullptr) {
        model->alterParameters();
    }
}

