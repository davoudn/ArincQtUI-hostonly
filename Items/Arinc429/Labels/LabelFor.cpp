#include "labelfor.h"
#include "generaldata.h"

#include  <QThread>
#include  <QTimer>

#define RATE_EVAL_TIME 1000

LabelFor<DReceiver>::LabelFor(str_t _path, str_t equipment_id, str_t label_id, QThread* thread):Label(_path, equipment_id, label_id) 
{
}
LabelFor<DReceiver>::LabelFor(str_t _path, str_t equipment_id, str_t label_id):Label(_path, equipment_id, label_id) 
{
}
LabelFor<DReceiver>::~LabelFor() 
{
}

inline bool LabelFor<DReceiver>::timeoutCheck()
{
    if (data_rate == 0){
        activate(false);
        return true;
    }
    return false;
}

value_t LabelFor<DReceiver>::valueBits(str_t pname)
{
    if (getIfActive())
    {
        return Label::valueBits(pname);
    }
    return value_t("--- No Data ---");
}

value_t LabelFor<DReceiver>::valueEng(str_t pname)
{
    if (getIfActive())
    {
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
    if (data_rate_counter == 0)
    {
        data_rate = 0;
    }
    else 
    {
        float r = (uint16_t)RATE_EVAL_TIME / data_rate_counter;
        if ( r > 2 * data_rate) {
        }
        // time_accumulation = 0.f;
    }
    resetDataRateCounter();
}

void LabelFor<DReceiver>::evalDataRate(double _dataRateTimerRestTime)
{
    if (data_rate_counter > 0)
    {
         data_rate = (uint16_t)_dataRateTimerRestTime / data_rate_counter;
    }
    else 
    {
        data_rate = 0;
    }
    resetDataRateCounter();
}

value_t  LabelFor<DReceiver>::ValueBits(str_t pname)
{
    if (GetIfActive())
    {
        return Label::ValueBits(pname);
    }
    return value_t("--- No Data ---");
}

value_t LabelFor<DReceiver>::ValueEng(str_t pname)
{
    if (GetIfActive())
    {
        return Label::ValueEng(pname);
    }
    return value_t("--- No Data ---");
}

void LabelFor<DReceiver>::SetValueBits(value_t _bits)
{
    Label::SetValueBits(_bits);
    IncrementDataRateCounter();
    Activate(true);
}

void LabelFor<DReceiver>::TimerCallback()
{
    EvalDataRate();
}

void LabelFor<DReceiver>::EvalDataRate()
{
    if (data_rate_counter==0)
    {
        data_rate = 0;
    }
    else 
    {
        float r = (uint16_t)RATE_EVAL_TIME / data_rate_counter;
        if ( r > 2 * data_rate) {
        }
    }
    ResetDataRateCounter();
}

void LabelFor<DReceiver>::EvalDataRate(double _dataRateTimerRestTime)
{
    if (data_rate_counter>0)
    {
         data_rate = (uint16_t)_dataRateTimerRestTime / data_rate_counter;
    }
    else 
    {
        data_rate = 0;
    }
    ResetDataRateCounter();
}
/*
    Reserver
*/
LabelFor<DReserved>::LabelFor():
    Label(GeneralData::getInstance()->LABELS_PATH, str_t(QString::number(1000)), str_t(QString::number(1000)))
{
}

value_t LabelFor<DReserved>::valueBits()
{
    return value_t(str_t(""));
}

value_t LabelFor<DReserved>::getDataRate()
{
    return value_t(str_t(""));
}

str_t LabelFor<DReserved>::getStatus()
{
    return str_t("");
}

str_t LabelFor<DReserved>::getName()
{
    return name;
}

LabelFor<DReserved>::LabelFor():
    Label(GeneralData::getInstance()->LABELS_PATH, str_t(QString
        ::number(1000)), str_t(QString::number(1000)))
{
}

value_t LabelFor<DReserved>::ValueBits()
{
    return value_t(str_t(""));
}

value_t LabelFor<DReserved>::GetDataRate()
{
    return value_t(str_t(""));
}

str_t LabelFor<DReserved>::GetStatus()
{
    return str_t("");
}

str_t LabelFor<DReserved>::GetName()
{
    return name;
}

/*
*/

LabelFor<DTransmitter>::LabelFor(str_t path, str_t equipment_id
    , str_t label_id):Label(path, equipment_id, label_id)
{
    dataRate = 25;
}
inline DArincData& LabelFor<DTransmitter>::getArincDataAndReset()
{
    setIfDataAvailable(false);
    return arincData;
}
void LabelFor<DTransmitter>::setIfDataAvailable(bool b)
{
    ifDataAvailable = b;
}
bool LabelFor<DTransmitter>::getIfDataAvailable()
{
    return ifDataAvailable;
}
void LabelFor<DTransmitter>::incrementDataRateCounter()
{
    if (ifActive)
    {
        Label::incrementDataRateCounter();
        if (dataRateCounter * MIN_TICK >= dataRate)
        {
            setIfDataAvailable(true);
            resetDataRateCounter();
        }
    }
    else 
    {
        setIfDataAvailable(false);
    }
}
void LabelFor<DTransmitter>::timerCallback()
{
}
