#ifndef LABELFOR_H
#define LABELFOR_H

#include "Label.h"
#include "types.h"

template <typename TYPE>
class LabelFor;
class QThread;
class QTimer;




template <>
class LabelFor<DReceiver>: public Label
{
public:
    LabelFor() = delete;
    LabelFor(str_t _path, str_t equipmentId, str_t labelId);
    LabelFor(str_t _path, str_t equipmentId, str_t labelId, QThread* thread);
    ~LabelFor() ;

    virtual value_t valueBits(str_t pname) override;
    virtual value_t valueEng(str_t pname) override;
    virtual void setValueBits(value_t _bits) override;
    virtual void timerCallback() override;
    // inlines ...
    inline bool timeoutCheck(){
        if (getDataRate() == 0){
            activate(false); // surplus
            return true;
        }
        return false;
    }


    void evalDataRate(double _dataRateTimerRestTime);

    void evalDataRate();

private:
    float timeAccumulation = 0.f;
};




template <>
class LabelFor<DTransmitter>: public Label
{
public:
    LabelFor(str_t _path, str_t equipmentId, str_t labelId):Label( _path,  equipmentId,  labelId){
        dataRate = 200;
    }

    inline DArincData& getArincDataAndReset(){
        setIfDataAvailable(false);
        return ArincData;
    }

    inline void setIfDataAvailable(bool _b){
        bIfDataAvailable = _b;
    }

    inline bool getIfDataAvailable(){
        return bIfDataAvailable;
    }

    void incrementDataRateCounter() override {
        if (bIfActive){
            Label::incrementDataRateCounter();
            if (dataRateCounter * MIN_TICK >= dataRate){
                setIfDataAvailable(true);
                resetDataRateCounter();
            }
        }
        else {
            setIfDataAvailable(false);
        }
    }
    virtual void timerCallback(){}
private:
    QTimer* sendDataRateTimer = nullptr;
    bool bIfDataAvailable = false;
};

template <>
class LabelFor<DReserved>: public Label
{
public:
    LabelFor();
    virtual value_t valueBits() override;
    virtual inline value_t getDataRate() override;
public:
    virtual str_t getStatus() override;
    virtual str_t getName() override;
};

#endif // LABELFOR_H
