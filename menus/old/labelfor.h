#ifndef LABELFOR_H
#define LABELFOR_H

#include "Label.h"
#include "types.h"
template <typename TYPE>
class LabelFor;



template <>
class LabelFor<DReceiver>: public Label
{
public:
    LabelFor(str_t _path, str_t equipmentId, str_t labelId);


    virtual value_t valueBits(str_t pname) override;
    virtual value_t valueEng(str_t pname) override;
    virtual void setValueBits(value_t _bits) override;

    // inlines ...
    inline bool timeoutCheckAndIncrement(double _timeOut){
        incrementTimeoutCounter();
        if (timeOutCounter>_timeOut){
            activate(false); // surplus
            return true;
        }
        return false;
    }

    inline void resetTimeout(){
        timeOutCounter = 0;
    }

    inline void resetRateCounter(){
        dataRateCounter = 0;

    }

    inline void incrementTimeoutCounter(){
        timeOutCounter++;
    }


    inline void incrementRateCounter(){
        dataRateCounter++;
    }

    inline value_t getDataRate(){
        return value_t(dataRate);
    }

public slots:
    inline void evalDataRate(){
        dataRate = dataRateCounter/dataRateTimerRestTime;
        resetRateCounter();
    }

private:
    QTimer* receiveDataRateTimer = nullptr;
    uint32_t dataRateCounter = 0;
    uint32_t timeOutCounter = 0;
    const double dataRateTimerRestTime = 1.f;  // second
};


template <>
class LabelFor<DTransmitter>: public Label
{
public:
    LabelFor(str_t _path, str_t equipmentId, str_t labelId);

    inline DArincData& getArincDataAndReset(){
        setIfDataAvailable(false);
        return ArincData;
    }

    inline void setIfDataAvailable(bool _b){
        bIfDataAvailable = _b;
    }

    inline bool getIfDataAvailable(){
        if ( getIfActive()){
            return bIfDataAvailable;
        }
        return false;
    }
    void setDataRate(value_t _value) override{
        dataRate = _value.toDouble();
        sendDataRateTimer->stop();
        if (dataRate!=0.0){
            dataRateTimerRestTime = 1.0 / dataRate;
        }
        sendDataRateTimer->start(dataRateTimerRestTime * SECONDS_TO_MILLISECONDS);
    }
public slots:
    inline void onDataAvailable(){
        setIfDataAvailable(true);
    }
private:
    QTimer* sendDataRateTimer = nullptr;
    uint32_t dataRateCounter = 0;
    bool bIfDataAvailable = false;
    double dataRateTimerRestTime = 0.2;  // second
};



#endif // LABELFOR_H
