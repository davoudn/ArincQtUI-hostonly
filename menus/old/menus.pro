QT += core gui  widgets serialport
HEADERS       = mainwindow.h \
    ATCBCDParameter.h \
    ArincData.h \
    BCDParameter.h \
    BNRParameter.h \
    BaseParameter.h \
    DEI1016.h \
    DEI1016RasberryConfigurations.h \
    DataSourceBase.h \
    DiscreteParameter.h \
    Equipment.h \
    FLNOParameter.h \
    FLOATBCDParameter.h \
    Handler.h \
    INTParameter.h \
    Label.h \
    PointerVector.h \
    Timer.h \
    TreeDataModel.h \
    arinc_functions.h \
    baseitem.h \
    bitutils.h \
    checkboxarray.h \
    devicedelegate.h \
    editordelegate.h \
    equipmentsids.h \
    labelfor.h \
    receiver.h \
    settingsdialog.h \
    transmitter.h \
    types.h \
    utils.h
SOURCES       = mainwindow.cpp \
                ATCBCDParameter.cpp \
                ArincData.cpp \
                BCDParameter.cpp \
                BNRParameter.cpp \
                BaseParameter.cpp \
                DEI1016.cpp \
                DEI1016RasberryConfigurations.cpp \
                DataSourceBase.cpp \
                DiscreteParameter.cpp \
                Equipment.cpp \
                FLNOParameter.cpp \
                FLOATBCDParameter.cpp \
                Handler.cpp \
                INTParameter.cpp \
                Label.cpp \
                Timer.cpp \
                TreeDataModel.cpp \
                arinc_functions.cpp \
                baseitem.cpp \
                bitutils.cpp \
                checkboxarray.cpp \
                devicedelegate.cpp \
                editordelegate.cpp \
                equipmentsids.cpp \
                labelfor.cpp \
                main.cpp \
                receiver.cpp \
                transmitter.cpp \
                utils.cpp \
                settingsdialog.cpp

# install
target.path = $$[QT_INSTALL_EXAMPLES]/widgets/mainwindows/menus
INSTALLS += target

RESOURCES += \
    icons.qrc

FORMS += \
    checkboxarray.ui \
    receiver.ui \
    settingsdialog.ui \
    transmitter.ui

DISTFILES += \
    equipment.json

