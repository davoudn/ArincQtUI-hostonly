#ifndef ACTION_H
#define ACTION_H
#include "types.h"
#include <stdint.h>
#include <array>

class BaseAction
{
protected:
    BaseAction() = delete;
    BaseAction(uint32_t dei, uint32_t channel) ;
    char data[POCKET_SIZE];
    uint32_t dei = 0;
    uint32_t channel = 0;


public:
    enum class ActionType{
        Data,
        Control
    };
    virtual ~BaseAction();
    virtual char* toPacket() = 0;
    bool if_applied = false;
    ActionType action_type;

};

class DataAction : public BaseAction
{
public:
    DataAction() = delete;
    DataAction(uint32_t dei, uint32_t ch, uint32_t tran_receive, uint32_t instr, uint32_t arinc_data, float rt);
    virtual char* toPacket() override;
protected:
    uint32_t arinc_data = 0;
    float rate = 0.f;
    uint32_t tran_receive = 0;
    uint32_t instruction = 0;
};

class ControlAction : public BaseAction
{
public:
    ControlAction() = delete;
    ControlAction(uint32_t dei_id, uint32_t control_word);
    virtual char* toPacket() override;
protected:
    uint32_t control_word = 0;
};

BaseAction* MakeDataAction(uint32_t device_id, uint32_t ch, uint32_t tran_receive, uint32_t instr, uint32_t arinc_data, float rt);
BaseAction* MakeControlAction(uint32_t device_id, uint32_t control_word);

class action
{
public:
    action() = delete;
    action(uint32_t ch, uint32_t trans_rec, uint32_t instr, uint32_t arinc_data, float rt, uint16_t control);

    bool if_applied = false;

    std::array<uint8_t,TRANSMMIT_PACKET_SIZE> data_array;
    char data[TRANSMMIT_PACKET_SIZE];

    float rate = 0.f;
    uint16_t control_word = 0;
    uint32_t arinc_data = 0;
    uint32_t instruction = 0;
    uint32_t channel = 0;
    uint32_t tran_receive = 0;
    void setData(uint32_t ch, uint32_t trans_rec, uint32_t instr, uint32_t arinc_data, float rt, uint16_t control);
    std::array<uint8_t,TRANSMMIT_PACKET_SIZE>& toArrayPacket();
    char* toPacket();


};

#endif // ACTION_H