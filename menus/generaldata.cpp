#include "generaldata.h"
GeneralData::GeneralData()
{
  //  LABELS_PATH = QString("/home/d/ARINC-QT/ArincQtUI-withlock-newtimer/menus/Labels/");
  //  TRANSMIT_CONFIGS_PATH = QString("/home/d/ARINC-QT/ArincQtUI-withlock-newtimer/menus/TransmitConfigs/");
  LABELS_PATH = QString("E:\\davoud\\projects\\ARINC-QT\\Source\\ArincQtUI-hostonly\\menus\\Labels\\");
  TRANSMIT_CONFIGS_PATH = QString("E:\\davoud\\projects\\ARINC-QT\\Source\\ArincQtUI-hostonly\\menus\\TransmitConfigs\\");
}
GeneralData* GeneralData::instance = nullptr;

GeneralData* GeneralData::getInstance()
{
    if (!instance){
        instance = new GeneralData();
    }
    return instance;
}
