#include "labelfor.h"

LabelFor<DReceiver>::LabelFor(str_t _path, str_t equipmentId, str_t labelId):Label(_path, equipmentId, labelId) {

    receiveDataRateTimer = new QTimer();
    receiveDataRateTimer->start(dataRateTimerRestTime * SECONDS_TO_MILLISECONDS);
    connect(receiveDataRateTimer, &QTimer::timeout, this, &LabelFor<DReceiver>::evalDataRate);

}


value_t  LabelFor<DReceiver>::valueBits(str_t pname){
    if (getIfActive()){
        return Label::valueBits(pname);
    }
    return value_t("--- No Data ---");
}

value_t LabelFor<DReceiver>::valueEng(str_t pname) {
    if (getIfActive()){
        return Label::valueEng(pname);
    }
    return value_t("--- No Data ---");
}

void LabelFor<DReceiver>::setValueBits(value_t _bits){
    Label::setValueBits(_bits);
    resetTimeout();
    incrementRateCounter();
}

LabelFor<DTransmitter>::LabelFor(str_t _path, str_t equipmentId, str_t labelId):Label(_path, equipmentId, labelId) {

    sendDataRateTimer = new QTimer();
    dataRate = 1.0/dataRateTimerRestTime;
    sendDataRateTimer->start(dataRateTimerRestTime * SECONDS_TO_MILLISECONDS);
    connect(sendDataRateTimer, &QTimer::timeout, this, &LabelFor<DTransmitter>::onDataAvailable);

}

