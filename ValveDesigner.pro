QT       += core gui printsupport help charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    circuit/accathodefollower.cpp \
    circuit/circuit.cpp \
    circuit/dccathodefollower.cpp \
    circuit/pentodecommoncathode.cpp \
    circuit/triodecommoncathode.cpp \
    main.cpp \
    designer.cpp \
    marker.cpp \
    parameter.cpp \
    spice/spice.cpp \
    valvemodel/device.cpp \
    valvemodel/model.cpp \
    valvemodel/models/cohenhelietriode.cpp \
    valvemodel/models/gardinerpentode.cpp \
    valvemodel/models/korenpentode.cpp \
    valvemodel/models/korentriode.cpp \
    valvemodel/models/reefmanpentode.cpp \
    valvemodel/models/simpletriode.cpp

HEADERS += \
    circuit/accathodefollower.h \
    circuit/circuit.h \
    circuit/dccathodefollower.h \
    circuit/pentodecommoncathode.h \
    circuit/triodecommoncathode.h \
    designer.h \
    marker.h \
    ngspice/sharedspice.h \
    parameter.h \
    spice/spice.h \
    valvemodel/device.h \
    valvemodel/model.h \
    valvemodel/models/cohenhelietriode.h \
    valvemodel/models/gardinerpentode.h \
    valvemodel/models/korenpentode.h \
    valvemodel/models/korentriode.h \
    valvemodel/models/reefmanpentode.h \
    valvemodel/models/simpletriode.h \
    valvemodel/valvemodel.h

FORMS += \
    designer.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32: LIBS += -L$$PWD/ngspice/ -lngspice

INCLUDEPATH += $$PWD/ngspice
DEPENDPATH += $$PWD/ngspice
