#pragma once

#include <QLabel>
#include <QLineEdit>
#include <QString>

/**
 * @brief The Parameter class
 *
 * Represents a single parameter of a model. This class is used to store the name of the parameter
 * and its value.
 */
class Parameter
{
public:
    Parameter(QString name, double value, bool isInput=false);

    double getValue() const;
    const QString &getName() const;
    double *getPointer();

    void setValue(double newValue);
    void setIsInput(bool newIsInput);

    void updateUI(QLabel *label, QLineEdit *value);

private:
    QString name;
    double value;
    bool isInput = false;
};
