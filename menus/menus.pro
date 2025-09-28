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
    action.h \
    actionsrecord.h \
    arinc_functions.h \
    baseitem.h \
    basetransiverwidget.h \
    bitutils.h \
    checkboxarray.h \
    editordelegate.h \
    equipmentsids.h \
    generaldata.h \
    labelfor.h \
    receiver.h \
    receiverworker.h \
    settingsdialog.h \
    transmitter.h \
    transmitterworker.h \
    transmmiter_setup_ui.h \
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
                action.cpp \
                actionsrecord.cpp \
                arinc_functions.cpp \
                baseitem.cpp \
                basetransiverwidget.cpp \
                bitutils.cpp \
                checkboxarray.cpp \
                editordelegate.cpp \
                equipmentsids.cpp \
                generaldata.cpp \
                labelfor.cpp \
                main.cpp \
                receiver.cpp \
                receiverworker.cpp \
                settingsdialog.cpp \
                transmitter.cpp \
                transmitterworker.cpp \
                transmmiter_setup_ui.cpp \
                utils.cpp

# install
target.path = ./menus
INSTALLS += target

RESOURCES += \
    icons.qrc

FORMS += \
    checkboxarray.ui \
    receiver.ui \
    receiver_setup.ui \
    settingsdialog.ui \
    transmitter.ui \
    transmmiter_setup_ui.ui

DISTFILES += \
    equipment.json

unix: LIBS += -lpigpio

QMAKE_CXXFLAGS += -O3
