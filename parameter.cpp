#include "parameter.h"

Parameter::Parameter(QString name, double value, bool isInput) : name(name), value(value), isInput(isInput)
{

}

double Parameter::getValue() const
{
    return value;
}

void Parameter::setValue(double newValue)
{
    value = newValue;
}

void Parameter::setIsInput(bool newIsInput)
{
    isInput = newIsInput;
}

void Parameter::updateUI(QLabel *label, QLineEdit *value)
{
    label->setVisible(true);
    label->setText(name);
    value->setVisible(true);
    value->setReadOnly(!isInput);
    if (!isInput) {
        label->setStyleSheet("QLabel { color : blue; }");
        value->setText(QString::number(this->value, 'f', 3));
    } else {
        label->setStyleSheet("QLabel { color : black; }");
        value->setText(QString::number(this->value, 'g', 9));
    }
}

const QString &Parameter::getName() const
{
    return name;
}

double *Parameter::getPointer()
{
    return &value;
}
