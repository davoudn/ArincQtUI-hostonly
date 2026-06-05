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
    LabelFor(str_t path, str_t equipment_id, str_t label_id);
    LabelFor(str_t path, str_t equipment_id, str_t label_id, QThread* thread);
    ~LabelFor() ;

    virtual value_t valueBits(str_t pname) override;
    virtual value_t valueEng(str_t pname) override;
    virtual void setValueBits(value_t bits) override;
    virtual void timerCallback() override;
    bool timeoutCheck();
    void evalDataRate(double dataRateTimerRestTime);
    void evalDataRate();

private:
    float timeAccumulation = 0.f;
};

template <>
class LabelFor<DTransmitter>: public Label
{
public:
    LabelFor(str_t path, str_t equipment_id
        , str_t label_id);
    DArincData& getArincDataAndReset();
    void setIfDataAvailable(bool b);
    bool getIfDataAvailable();
    void incrementDataRateCounter() override;
    virtual void timerCallback(){}
private:
    QTimer* sendDataRateTimer = nullptr;
    bool ifDataAvailable = false;
};

template <>
class LabelFor<DReserved>: public Label
{
public:
    LabelFor();
    virtual value_t ValueBits() override;
    virtual inline value_t GetDataRate() override;
public:
    virtual str_t GetStatus() override;
    virtual str_t GetName() override;
};

#endif // LABELFOR_H