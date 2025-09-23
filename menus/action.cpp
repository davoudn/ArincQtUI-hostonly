#include "action.h"
#include "bitutils.h"
#include <bitset>


BaseAction::BaseAction(uint32_t deviceid, uint32_t ch):deviceId(deviceid), chanel(ch)
{
    for(int i=0; i < TRANSMMIT_PACKET_SIZE; i++)
    {
        data[i]=static_cast<char>(0);
    }
}

BaseAction::~BaseAction(){

}


DataAction::DataAction(uint32_t deviceid, uint32_t ch, uint32_t trancive, uint32_t instr, uint32_t arincdata, float rt):BaseAction(deviceid, ch),
    arincData(arincdata), rate(rt)
{
    actionType = BaseAction::ActionType::Data;
}

char* DataAction::toPacket()
{
    dword_t a{arincData};
    AUX::convertFromArincToDEI(a);
    uint32_t x = static_cast<uint32_t>(a.to_ulong());
    data[INSTRUCTION_BYTE] = AUX::makeInstructionByte(chanel, instruction, tranReceive);
    data[RATE_BYTE] = AUX::timeToBits(rate);
    AUX::split(x, data[ARINC_BYTE0], data[ARINC_BYTE1], data[ARINC_BYTE2], data[ARINC_BYTE3]);
    return data;
}


ControlAction::ControlAction(uint32_t deviceid, uint32_t ch, uint32_t controlword):BaseAction(ch,deviceid), controlWord(controlword)
{
    actionType = BaseAction::ActionType::Control;
}

char* ControlAction::toPacket()
{
    data[INSTRUCTION_BYTE] = AUX::makeInstructionByte(chanel, Instructions::APPLY_CONTROL_WORD, 0);
    AUX::split(controlWord, data[CONTROL_BYTE0], data[CONTROL_BYTE1]);
    return data;
}


BaseAction* MakeDataAction(uint32_t deviceid, uint32_t ch, uint32_t trancive, uint32_t instr, uint32_t arincdata, float rt){
    return new DataAction( deviceid,  ch,  trancive, instr, arincdata, rt);
}

BaseAction* MakeControlAction(uint32_t deviceid, uint32_t ch, uint32_t controlword){
    return new ControlAction( deviceid,  ch,  controlword);
}



action::action(uint32_t ch, uint32_t trans_rec, uint32_t instr,
                                                              uint32_t arincdata, float rt, uint16_t control)
 {
    setData(ch,  trans_rec,  instr,  arincdata,  rt,  control);
 }

void action::setData(uint32_t ch, uint32_t transreceive, uint32_t instr,
                                                              uint32_t arincdata, float rt, uint16_t control)
{
    chanel = ch;
    tranReceive = transreceive;
    instruction = instr;
    arincData   = arincdata;
    rate        = rt;
    controlWord = control;
}

char* action::toPacket()
{
    qInfo() << "**Arinc --> "<<std::bitset<32>(arincData).to_string( );
    dword_t a{arincData};
    AUX::convertFromArincToDEI(a);
    qInfo() << "**DEI   --> "<<a.to_string( );
    uint32_t x = static_cast<uint32_t>(a.to_ulong());
    data[INSTRUCTION_BYTE] = AUX::makeInstructionByte(chanel, instruction, tranReceive);
    data[RATE_BYTE] = AUX::timeToBits(rate);
    AUX::split(x, data[ARINC_BYTE0], data[ARINC_BYTE1], data[ARINC_BYTE2], data[ARINC_BYTE3]);
    uint16_t controlWord = 0;
    AUX::split(controlWord, data[CONTROL_BYTE0], data[CONTROL_BYTE1]);
    return data;
}
