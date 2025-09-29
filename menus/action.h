#ifndef ACTION_H
#define ACTION_H
#include "types.h"
#include <stdint.h>
#include <array>

class BaseAction
{
protected:
    BaseAction();
    BaseAction(uint32_t dei, uint32_t chanel) ;
    record_t data;
    uint32_t dei = 0;
    uint32_t chanel = 0;


public:
    enum class ActionType{
        Data,
        Control
    };

    virtual ~BaseAction();
    virtual void toData() = 0;
    virtual record_t& toPacket() = 0;

    bool bIfApplied = false;
    ActionType actionType;

};

class ResetBoard : public BaseAction {
public:
    virtual void toData()  override;
    virtual record_t& toPacket() override;
};

class DataAction : public BaseAction
{
public:
    DataAction() = delete;
    DataAction(uint32_t dei, uint32_t ch, uint32_t trancive, uint32_t instr, uint32_t arincdata, float rt);
    ~DataAction();
    record_t& toPacket() override;

    void toData() override;
    void init(char*) ;
    void init(uint32_t dei, uint32_t ch, uint32_t trancive, uint32_t instr, uint32_t arincdata, float rt) ;

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
    virtual record_t& toPacket() override;
    void init(uint32_t dei, uint32_t ch, uint32_t tr1ancive, uint32_t instr, uint32_t arincdata, float rt) ;
protected:
    uint32_t controlWord = 0;
};

BaseAction* MakeDataAction(uint32_t deviceid, uint32_t ch, uint32_t trancive, uint32_t instr, uint32_t arincdata, float rt);
BaseAction* MakeControlAction(uint32_t deviceid, uint32_t controlword);


#endif // ACTION_H
