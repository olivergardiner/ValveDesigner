#pragma once

#include "circuit/circuit.h"
#include "valvemodel/device.h"
#include "valvemodel/valvemodel.h"
#include "marker.h"

#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class Designer;
}
QT_END_NAMESPACE

class Designer : public QMainWindow
{
    Q_OBJECT

public:
    Designer(QWidget *parent = nullptr);
    ~Designer();

public slots:
    void on_seriesHovered(const QPointF &point, bool state);

private slots:
    void on_actionTriode_Common_Cathode_triggered();

    void on_actionPentode_Common_Cathode_triggered();

    void on_deviceSelect_currentIndexChanged(int index);

    void on_par1Value_editingFinished();

    void on_par2Value_editingFinished();

    void on_par3Value_editingFinished();

    void on_par4Value_editingFinished();

    void on_par5Value_editingFinished();

    void on_par6Value_editingFinished();

    void on_par7Value_editingFinished();

    void on_par8Value_editingFinished();

    void on_actionAC_Cathode_Follower_triggered();

    void on_actionDC_Cathode_Follower_triggered();

private:
    Ui::Designer *ui;  

    QList<Device *> triodes;
    QList<Device *> pentodes;
    QList<Circuit *> circuits;

    Circuit *currentCircuit;
    bool isTriodeCircuit;
    bool isDeviceSelectBuilt = false;

    QActionGroup *circuitGroup;
    QLabel *parameterLabels[16];
    QLineEdit *parameterValues[16];
    Marker *marker;

    void loadDevices();
    void selectCircuit(eCircuit type);
    void buildDeviceSelect(QList<Device *> devices);
    void buildParameterLists();
    void buildCircuitGroup();
    void updateCircuit();
    void changeCircuit(eCircuit type, eDeviceType device);
};
