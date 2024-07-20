#include "model.h"
#include "valvemodel/models/cohenhelietriode.h"
#include "valvemodel/models/gardinerpentode.h"
#include "valvemodel/models/korentriode.h"
#include "valvemodel/models/reefmanpentode.h"
#include "valvemodel/models/simpletriode.h"
#include "valvemodel/models/korenpentode.h"
#include "valvemodel/valvemodel.h"

Model::Model()
{
    parameter[PAR_KG1] = new Parameter("Kg:", 0.7);
    parameter[PAR_VCT] = new Parameter("Vct:", 0.1);
    parameter[PAR_X] = new Parameter("X:", 1.5);
    parameter[PAR_MU] = new Parameter("Mu:", 100.0);
    parameter[PAR_KP] = new Parameter("Kp:", 500.0);
    parameter[PAR_KVB] = new Parameter("Kvb:", 300.0);
    parameter[PAR_KVB1] = new Parameter("Kvb2:", 30.0);

    parameter[PAR_KG2] = new Parameter("Kg2:", 0.15);
    parameter[PAR_KG2A] = new Parameter("Kg3:", 0.15);
    parameter[PAR_A] = new Parameter("A:", 0.0);
    parameter[PAR_ALPHA] = new Parameter("Alpha:", 0.0);
    parameter[PAR_BETA] = new Parameter("Beta:", 0.0);
    parameter[PAR_GAMMA] = new Parameter("Gamma:", 1.0);
    parameter[PAR_OS] = new Parameter("Os:", 0.01);

    parameter[PAR_TAU] = new Parameter("Tau:", 0.1);
    parameter[PAR_RHO] = new Parameter("Rho:", 0.1);
    parameter[PAR_THETA] = new Parameter("Theta:", 0.1);
    parameter[PAR_PSI] = new Parameter("Psi:", 0.1);

    parameter[PAR_OMEGA] = new Parameter("Omega:", 30.0);
    parameter[PAR_LAMBDA] = new Parameter("Lambda:", 30.0);
    parameter[PAR_NU] = new Parameter("Nu:", 0.0);
    parameter[PAR_S] = new Parameter("S:", 0.0);
    parameter[PAR_AP] = new Parameter("Ap:", 0.0);
}

/**
 * @brief Model::screenCurrent
 * @param va The anode voltage
 * @param vg1 The grid voltage
 * @param vg2 For pentodes, the screen grid voltage
 * @param secondaryEmission Whether to include secondary emission in the calculation
 * @return The screen current
 *
 * This default implementation of the screenCurrent method returns 0.0 so that its definition
 * is not required for triodes
 */
double Model::screenCurrent(double va, double vg1, double vg2, bool secondaryEmission)
{
    return 0.0;
}

double Model::cathodeCurrent(double va, double vg1, double vg2, bool secondaryEmission)
{
    return anodeCurrent(va, vg1, vg2, secondaryEmission) + screenCurrent(va, vg1, vg2, secondaryEmission);
}

/**
 * @brief Model::anodeVoltage
 * @param ia The desired anode current
 * @param vg1 The grid voltage
 * @param vg2 For pentodes, the screen grid voltage
 * @return The anode voltage that will result in the desired anode current
 *
 * Uses a gradient based search to find the anode voltage that will result in the specified
 * anode current given the specified grid voltages. This is provided to enable the accurate
 * determination of a cathode load line.
 */
double Model::anodeVoltage(double ik, double vg1, double vg2, bool secondaryEmission)
{
    double va = 100.0;
    double tolerance = 1.2;

    double ikTest = anodeCurrent(va, vg1, vg2, secondaryEmission);
    double gradient = 100.0 * (anodeCurrent(va + 0.01, vg1, vg2, secondaryEmission) - ikTest);
    //double ikTest = cathodeCurrent(va, vg1, vg2, secondaryEmission);
    //double gradient = 100.0 * (cathodeCurrent(va + 0.01, vg1, vg2, secondaryEmission) - ikTest);
    double ikErr = ik - ikTest;

    int count = 0;
    while (abs(ikErr) > 0.005 && count++ < 1000) {
        if (gradient != 0.0) {
            double vaNext = va + ikErr / gradient;
            if (vaNext < 0.0) {
                vaNext = 0.0;
            }
            if (vaNext < va / tolerance) { // use the gradient but limit step to tolerance
                vaNext = va / tolerance;
            }
            if (vaNext > tolerance * va) { // use the gradient but limit step to tolerance
                vaNext = tolerance * va;
            }
            va = vaNext;
        } else {
            break;
        }
        ikTest = anodeCurrent(va, vg1, vg2, secondaryEmission);
        //ikTest = cathodeCurrent(va, vg1, vg2, secondaryEmission);
        gradient = 100.0 * (anodeCurrent(va + 0.01, vg1, vg2, secondaryEmission) - ikTest);
        ikErr = ik - ikTest;
    }

    return va;
}

double Model::screenVoltage(double ik, double va, double vg1, bool secondaryEmission)
{
    double vg2 = 100.0;
    double tolerance = 1.2;

    double ikTest = cathodeCurrent(va, vg1, vg2, secondaryEmission);
    double gradient = 100.0 * (cathodeCurrent(va, vg1, vg2+0.01, secondaryEmission) - ikTest);
    double ikErr = ik - ikTest;

    int count = 0;
    while (abs(ikErr) > 0.005 && count++ < 1000) {
        if (gradient != 0.0) {
            double vg2Next = vg2 + ikErr / gradient;
            if (vg2Next < 0.0) {
                vg2Next = 0.0;
            }
            if (vg2Next < vg2 / tolerance) { // use the gradient but limit step to tolerance
                vg2Next = vg2 / tolerance;
            }
            if (vg2Next > tolerance * vg2) { // use the gradient but limit step to tolerance
                vg2Next = tolerance * vg2;
            }
            vg2 = vg2Next;
        } else {
            break;
        }
        ikTest = cathodeCurrent(va, vg1, vg2, secondaryEmission);
        gradient = 100.0 * (cathodeCurrent(va, vg1, vg2 + 0.01, secondaryEmission) - ikTest);
        ikErr = ik - ikTest;
    }

    return vg2;
}

double Model::getParameterValue(int parameterIndex)
{
    return parameter[parameterIndex]->getValue();
}

Parameter *Model::getParameter(int parameterIndex)
{
    return parameter[parameterIndex];
}

/**
 * @brief Model::fromJson
 * @param source
 * @return
 */
Model *Model::fromJson(QJsonObject source)
{
    Model *model = nullptr;

    QString device;
    if (source.contains("device") && source["device"].isString()) {
        device = source["device"].toString();
    } else {
        return nullptr;
    }

    QString type;
    if (source.contains("type") && source["type"].isString()) {
        type = source["type"].toString();
    } else {
        return nullptr;
    }

    if (device == "triode") {
        if (type == "simple") {
            model = new SimpleTriode();
        }
        if (type == "koren") {
            model = new KorenTriode();
        }
        if (type == "cohenHelie") {
            model = new CohenHelieTriode();
        }
    }

    if (device == "pentode") {
        if (type == "koren") {
            model = new KorenPentode();
        }
        if (type == "reefman") {
            model = new ReefmanPentode();
        }
        if (type == "gardiner") {
            model = new GardinerPentode();
        }
    }

    if (model != nullptr) {
        model->_fromJson(source);
    }

    return model;
}
