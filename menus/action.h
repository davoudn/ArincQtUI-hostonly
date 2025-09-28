#ifndef ACTION_H
#define ACTION_H
#include "types.h"
#include <stdint.h>
#include <array>

class BaseAction
{
protected:
    BaseAction() = delete;
    BaseAction(uint32_t dei, uint32_t chanel) ;
    char data[FRAME_POCKET_SIZE];
    uint32_t dei = 0;
    uint32_t chanel = 0;


public:
    enum class ActionType{
        Data,
        Control
    };
    virtual void init(char*) = 0;
    virtual void init(uint32_t dei, uint32_t ch, uint32_t trancive, uint32_t instr, uint32_t arincdata, float rt) = 0;

    virtual ~BaseAction();
    virtual void toData() = 0;
    virtual char* toPacket() = 0;
    bool bIfApplied = false;
    ActionType actionType;

};

class DataAction : public BaseAction
{
public:
    DataAction() = delete;
    DataAction(uint32_t dei, uint32_t ch, uint32_t trancive, uint32_t instr, uint32_t arincdata, float rt);
    ~DataAction();
    char* toPacket() override;
    void toData() override;
    void init(char*) override;
    void init(uint32_t dei, uint32_t ch, uint32_t trancive, uint32_t instr, uint32_t arincdata, float rt) override;

protected:
    uint32_t arincData = 0;
    float rate = 0.f;
    uint32_t tranReceive = 0;
    uint32_t instruction = 0;
};

class ControlAction : public BaseAction
{
public:
    ControlAction() = delete;
    ControlAction(uint32_t deiId, uint32_t controlword);
    ~ControlAction();
    virtual void toData()  override;
    virtual char* toPacket() override;
    virtual void init(char*) override;
    virtual void init(uint32_t dei, uint32_t ch, uint32_t trancive, uint32_t instr, uint32_t arincdata, float rt) override;
protected:
    uint32_t controlWord = 0;
};

BaseAction* MakeDataAction(uint32_t deviceid, uint32_t ch, uint32_t trancive, uint32_t instr, uint32_t arincdata, float rt);
BaseAction* MakeControlAction(uint32_t deviceid, uint32_t controlword);

class action
{
public:
    action() = delete;
    action(uint32_t ch, uint32_t trans_rec, uint32_t instr, uint32_t arincdata, float rt, uint16_t control);

    bool bIfApplied = false;

    std::array<uint8_t,TX_BUFFER_SIZE> dataArray;
    char data[TX_BUFFER_SIZE];

    float rate = 0.f;
    uint16_t controlWord = 0;
    uint32_t arincData = 0;
    uint32_t instruction = 0;
    uint32_t chanel = 0;
    uint32_t tranReceive = 0;
    void setData(uint32_t ch, uint32_t trans_rec, uint32_t instr, uint32_t arincdata, float rt, uint16_t control);
    std::array<uint8_t,TX_BUFFER_SIZE>& toArrayPacket();
    char* toPacket();

};

#endif // ACTION_H
