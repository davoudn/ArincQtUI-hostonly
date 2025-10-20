#include "generaldata.h"
GeneralData::GeneralData()
{
   // LABELS_PATH = QString("/home/orangepi/ARINC-QT/ArincQtUI-withlock-newtimer/menus/Labels/");
  //  TRANSMIT_CONFIGS_PATH = QString("/home/orangepi/ARINC-QT/ArincQtUI-withlock-newtimer/menus/TransmitConfigs/");
#ifdef ORANGEPI
  LABELS_PATH = QString("/home/orangepi/ARINC-QT/ArincQtUI-hostonly/menus/Labels/");
  TRANSMIT_CONFIGS_PATH = QString("/home/orangepi/ARINC-QT/ArincQtUI-hostonly/menus/TransmitConfigs/");
#endif
#ifdef RASBERRYPI
  LABELS_PATH = QString("/home/d/ARINC-QT/ArincQtUI-hostonly/menus/Labels/");
  TRANSMIT_CONFIGS_PATH = QString("/home/d/ARINC-QT/ArincQtUI-hostonly/menus/TransmitConfigs/");
#endif
  //LABELS_PATH = QString("E:\\davoud\\projects\\ARINC-QT\\Source\\ArincQtUI-hostonly\\menus\\Labels\\");
  //TRANSMIT_CONFIGS_PATH = QString("E:\\davoud\\projects\\ARINC-QT\\Source\\ArincQtUI-hostonly\\menus\\TransmitConfigs\\");
}
GeneralData* GeneralData::instance = nullptr;

GeneralData* GeneralData::getInstance()
{
    if (!instance){
        instance = new GeneralData();
    }
    return instance;
}

PointerVector<BaseAction>& GeneralData::getActions()
{
    return actions;
}
