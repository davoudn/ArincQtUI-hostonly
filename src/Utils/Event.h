#ifndef EVENT_H
#define EVENT_H
#include "Utils/Types.h"
#include <stdint.h>
#include <array>

class BaseEvent
{
protected:
    BaseEvent() = delete;
    BaseEvent(uint32_t dei, uint32_t channel) ;
    char data[POCKET_SIZE];
    uint32_t dei = 0;
    uint32_t channel = 0;


public:
    enum class EventType{
        Data,
        Control
    };
    virtual ~BaseEvent();
    virtual char* toPacket() = 0;
    bool if_applied = false;
    EventType event_type;

};


class DataEvent : public BaseEvent
{
public:
    DataEvent() = delete;
    DataEvent(uint32_t dei, uint32_t ch, uint32_t tran_receive, uint32_t instr, uint32_t arinc_data, float rt);
    virtual char* toPacket() override;
protected:
    uint32_t arinc_data = 0;
    float rate = 0.f;
    uint32_t tran_receive = 0;
    uint32_t instruction = 0;
};

class ControlEvent : public BaseEvent
{
public:
    ControlEvent() = delete;
    ControlEvent(uint32_t dei_id, uint32_t control_word);
    virtual char* toPacket() override;
protected:
    uint32_t control_word = 0;
};

using BaseEventPtr = std::unique_ptr<BaseEvent>;
using DataEventPtr = std::unique_ptr<DataEvent>;
using ControlEventPtr = std::unique_ptr<ControlEven>;

#endif // EVENT_H
