#include "designer.h"
#include "circuit/pentodecommoncathode.h"
#include "circuit/triodecommoncathode.h"
#include "circuit/accathodefollower.h"
#include "circuit/dccathodefollower.h"
#include "spice/spice.h"
#include "ui_designer.h"
#include "valvemodel/valvemodel.h"

#include <QActionGroup>
#include <QDir>
#include <QJsonDocument>

Designer::Designer(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Designer)
{
    //ngSpice_Init(ng_getchar, ng_getstat, ng_exit, ng_data, ng_initdata, ng_thread_runs, NULL);

    loadDevices();

    circuits.append(new TriodeCommonCathode(this));
    circuits.append(new PentodeCommonCathode(this));
    circuits.append(new ACCathodeFollower(this));
    circuits.append(new DCCathodeFollower(this));

    ui->setupUi(this);

    marker = new Marker(ui->chartView->chart());
    marker->hide();

    buildCircuitGroup();

    buildParameterLists();

    on_actionTriode_Common_Cathode_triggered();
}

Designer::~Designer()
{
    delete ui;
}

void Designer::loadDevices()
{
    QString modelPath = tr("../models");
    QDir modelDir(modelPath);

    QStringList filters;
    filters << "*.vwm";
    modelDir.setNameFilters(filters);

    QStringList models = modelDir.entryList();

    for (int i = 0; i < models.size(); i++) {
        QString modelFileName = modelPath + "/" + models.at(i);
        QFile modelFile(modelFileName);
        if (!modelFile.open(QIODevice::ReadOnly)) {
            qWarning("Couldn't open model file: ", modelFile.fileName().toStdString().c_str());
        }
        else {
            QByteArray modelData = modelFile.readAll();
            QJsonDocument modelDoc(QJsonDocument::fromJson(modelData));

            if (modelDoc.isObject()) {
                Device *model = new Device(modelDoc.object());
                if (model->getModel()->getDeviceType() == TRIODE) {
                    triodes.append(model);
                }

                if (model->getModel()->getDeviceType() == PENTODE) {
                    pentodes.append(model);
                }
            }
        }
    }
}

void Designer::buildDeviceSelect(QList<Device *> devices)
{
    ui->deviceSelect->clear();
    foreach (Device *device, devices) {
        ui->deviceSelect->addItem(device->getName());
    }

    isDeviceSelectBuilt = true;
}

void Designer::buildParameterLists()
{
    parameterLabels[0] = ui->par1Name;
    parameterLabels[1] = ui->par2Name;
    parameterLabels[2] = ui->par3Name;
    parameterLabels[3] = ui->par4Name;
    parameterLabels[4] = ui->par5Name;
    parameterLabels[5] = ui->par6Name;
    parameterLabels[6] = ui->par7Name;
    parameterLabels[7] = ui->par8Name;
    parameterLabels[8] = ui->par9Name;
    parameterLabels[9] = ui->par10Name;
    parameterLabels[10] = ui->par11Name;
    parameterLabels[11] = ui->par12Name;
    parameterLabels[12] = ui->par13Name;
    parameterLabels[13] = ui->par14Name;
    parameterLabels[14] = ui->par15Name;
    parameterLabels[15] = ui->par16Name;

    parameterValues[0] = ui->par1Value;
    parameterValues[1] = ui->par2Value;
    parameterValues[2] = ui->par3Value;
    parameterValues[3] = ui->par4Value;
    parameterValues[4] = ui->par5Value;
    parameterValues[5] = ui->par6Value;
    parameterValues[6] = ui->par7Value;
    parameterValues[7] = ui->par8Value;
    parameterValues[8] = ui->par9Value;
    parameterValues[9] = ui->par10Value;
    parameterValues[10] = ui->par11Value;
    parameterValues[11] = ui->par12Value;
    parameterValues[12] = ui->par13Value;
    parameterValues[13] = ui->par14Value;
    parameterValues[14] = ui->par15Value;
    parameterValues[15] = ui->par16Value;
}

void Designer::buildCircuitGroup()
{
    circuitGroup = new QActionGroup(this);
    circuitGroup->addAction(ui->actionTriode_Common_Cathode);
    circuitGroup->addAction(ui->actionAC_Cathode_Follower);
    circuitGroup->addAction(ui->actionDC_Cathode_Follower);
    circuitGroup->addAction(ui->actionLTP_Phase_Splitter);
    circuitGroup->addAction(ui->actionCathodine_Phase_Splitter);
    circuitGroup->addAction(ui->actionPentode_Common_Cathode);
    circuitGroup->addAction(ui->actionTriode_SE_Output);
    circuitGroup->addAction(ui->actionPentode_SE_Output);
    circuitGroup->addAction(ui->actionPentode_SE_Output_UL);
    circuitGroup->addAction(ui->actionTriode_PP_Output);
    circuitGroup->addAction(ui->actionPentode_PP_Output);
    circuitGroup->addAction(ui->actionPentode_PP_Output_UL);
}

void Designer::updateCircuit()
{
    ui->chartView->chart()->removeAllSeries();
    currentCircuit->updateParameters(parameterLabels, parameterValues);
    currentCircuit->calculateOperatingPoint(ui->chartView);
    currentCircuit->showAnodeCharacteristics(ui->chartView);
    currentCircuit->updateParameters(parameterLabels, parameterValues);
}

void Designer::changeCircuit(eCircuit type, eDeviceType device)
{
    isDeviceSelectBuilt = false;
    currentCircuit = circuits.at(type);
    isTriodeCircuit = (device == TRIODE);
    if (isTriodeCircuit) {
        buildDeviceSelect(triodes);
        currentCircuit->setDevice(triodes.at(ui->deviceSelect->currentIndex()));
    } else {
        buildDeviceSelect(pentodes);
        currentCircuit->setDevice(pentodes.at(ui->deviceSelect->currentIndex()));
    }

    currentCircuit->showDescription(ui->textBrowser);
    updateCircuit();
}

void Designer::on_actionTriode_Common_Cathode_triggered()
{
    changeCircuit(TRIODE_COMMON_CATHODE, TRIODE);
}


void Designer::on_actionPentode_Common_Cathode_triggered()
{
    changeCircuit(PENTODE_COMMON_CATHODE, PENTODE);
}

void Designer::on_actionAC_Cathode_Follower_triggered()
{
    changeCircuit(AC_CATHODE_FOLLOWER, TRIODE);
}

void Designer::on_actionDC_Cathode_Follower_triggered()
{
    changeCircuit(DC_CATHODE_FOLLOWER, TRIODE);
}

void Designer::on_deviceSelect_currentIndexChanged(int index)
{
    if (isDeviceSelectBuilt) {
        if (isTriodeCircuit) {
            currentCircuit->setDevice(triodes.at(index));
        } else {
            currentCircuit->setDevice(pentodes.at(index));
        }
        updateCircuit();
    }
}


void Designer::on_par1Value_editingFinished()
{
    currentCircuit->parameterChanged(0, ui->par1Value->text());
    updateCircuit();
}


void Designer::on_par2Value_editingFinished()
{
    currentCircuit->parameterChanged(1, ui->par2Value->text());
    updateCircuit();
}


void Designer::on_par3Value_editingFinished()
{
    currentCircuit->parameterChanged(2, ui->par3Value->text());
    updateCircuit();
}


void Designer::on_par4Value_editingFinished()
{
    currentCircuit->parameterChanged(3, ui->par4Value->text());
    updateCircuit();
}


void Designer::on_par5Value_editingFinished()
{
    currentCircuit->parameterChanged(4, ui->par5Value->text());
    updateCircuit();
}


void Designer::on_par6Value_editingFinished()
{
    currentCircuit->parameterChanged(5, ui->par6Value->text());
    updateCircuit();
}


void Designer::on_par7Value_editingFinished()
{
    currentCircuit->parameterChanged(6, ui->par7Value->text());
    updateCircuit();
}


void Designer::on_par8Value_editingFinished()
{
    currentCircuit->parameterChanged(7, ui->par8Value->text());
    updateCircuit();
}

void Designer::on_seriesHovered(const QPointF &point, bool state)
{
    QXYSeries *series = qobject_cast<QXYSeries *>(QObject::sender());
    if (series != nullptr) {
        if (state) {
            QString text = series->name() + QString("\nVa: %1\nIa: %2").arg(point.x()).arg(point.y());
            marker->setText(text);
            marker->setAnchor(point);
            marker->setZValue(11);
            marker->updateGeometry();
            marker->show();
        } else {
            marker->hide();
        }
    }
}


