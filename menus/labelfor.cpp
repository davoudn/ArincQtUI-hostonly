#include "labelfor.h"
#include "generaldata.h"

#include  <QThread>
#include  <QTimer>

#define RATE_EVAL_TIME 1000


LabelFor<DReceiver>::LabelFor(str_t _path, str_t equipmentId, str_t labelId, QThread* thread):Label(_path, equipmentId, labelId) {


}

LabelFor<DReceiver>::LabelFor(str_t _path, str_t equipmentId, str_t labelId):Label(_path, equipmentId, labelId) {

}
LabelFor<DReceiver>::~LabelFor() {


}

value_t  LabelFor<DReceiver>::valueBits(str_t pname)
{
    if (getIfActive()){
        return Label::valueBits(pname);
    }
    return value_t("--- No Data ---");
}

value_t LabelFor<DReceiver>::valueEng(str_t pname)
{
    if (getIfActive()){
        return Label::valueEng(pname);
    }
    return value_t("--- No Data ---");
}

void LabelFor<DReceiver>::setValueBits(value_t _bits)
{
    Label::setValueBits(_bits);
    incrementDataRateCounter();
    activate(true);
}
void LabelFor<DReceiver>::timerCallback()
{
    evalDataRate();
}

void LabelFor<DReceiver>::evalDataRate()
{
   // qDebug() << "LabelFor<DReceiver>::evalDataRate(), eval rate!!";
    if (dataRateCounter>0){
         dataRate = (uint16_t)RATE_EVAL_TIME / dataRateCounter;
         timeAccumulation = 0.f;
    }
    else {
        dataRate = 0;
    }
    resetDataRateCounter();

}

void LabelFor<DReceiver>::evalDataRate(double _dataRateTimerRestTime)
{
    if (dataRateCounter>0){
         dataRate = (uint16_t)_dataRateTimerRestTime / dataRateCounter;
    }
    else {
        dataRate = 0;
    }
    resetDataRateCounter();

}

/*
 *
 *
 *
 */

//LabelFor<DTransmitter>::LabelFor(str_t _path, str_t equipmentId, str_t labelId):Label(_path, equipmentId, labelId) {

//    dataRate = 25;
     /*
    sendDataRateTimer = new QTimer();
    sendDataRateTimer->start(dataRateTimerRestTime * SECONDS_TO_MILLISECONDS);
    connect(sendDataRateTimer, &QTimer::timeout, this, &LabelFor<DTransmitter>::onDataAvailable);
    */
//}

LabelFor<DReserved>::LabelFor():Label( GeneralData::getInstance()->LABELS_PATH, str_t(QString::number(1000)), str_t(QString::number(1000)))
{
}
//
value_t LabelFor<DReserved>::valueBits(){
    return value_t(str_t(""));
}
value_t LabelFor<DReserved>::getDataRate(){
    return value_t(str_t(""));
}
str_t LabelFor<DReserved>::getStatus(){
    return str_t("");
}
str_t LabelFor<DReserved>::getName(){
    return Name;
}
