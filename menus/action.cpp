#include "action.h"
#include "bitutils.h"


BaseAction::BaseAction(uint32_t deiId, uint32_t ch):dei(deiId), chanel(ch)
{
    for(int i=0; i < TX_BUFFER_SIZE; i++)
    {
        data[i]=static_cast<char>(0);
    }
}

BaseAction::~BaseAction(){

}
BaseAction::BaseAction(){

}


void ResetBoard::toData()
{
}
record_t& ResetBoard::toPacket()
{
    uint32_t x =0;
    data[INITIAL_BYTE_INDEX] = INITIAL_BYTE;
    data[FINAL_BYTE_INDEX] = FINAL_BYTE;
    data[INSTRUCTION_BYTE] = AUX::makeInstructionByte(0, 0, Instructions::CLEAR_ALL, 0);
    data[RATE_BYTE] = 0;
    AUX::split(x, data[ARINC_BYTE0], data[ARINC_BYTE1], data[ARINC_BYTE2], data[ARINC_BYTE3]);
    AUX::log(data,"Reset board.");
    return data;
}

DataAction::DataAction(uint32_t dei, uint32_t ch, uint32_t trancive, uint32_t instr, uint32_t arincdata, float rt):BaseAction(dei, ch),
    arincData(arincdata), rate(rt), tranReceive(trancive), instruction(instr)
{
    actionType = BaseAction::ActionType::Data;
}
DataAction::~DataAction()
{

}


void  DataAction::init(char* bytes)
{

}

void DataAction::init(uint32_t dei, uint32_t ch, uint32_t trancive, uint32_t instr, uint32_t arincdata, float rt)
{

}
void  DataAction::toData() {

}
record_t& DataAction::toPacket()
{
    dword_t a{arincData};
    AUX::convertFromArincToDEI(a);
    uint32_t x = static_cast<uint32_t>(a.to_ulong());
    data[INITIAL_BYTE_INDEX] = INITIAL_BYTE;
    data[FINAL_BYTE_INDEX] = FINAL_BYTE;
    data[INSTRUCTION_BYTE] = AUX::makeInstructionByte(dei, 0, instruction, tranReceive);
    data[RATE_BYTE] = AUX::timeToBits(rate);
    AUX::split(x, data[ARINC_BYTE0], data[ARINC_BYTE1], data[ARINC_BYTE2], data[ARINC_BYTE3]);
    AUX::log(data,"Data action. ");

    return data;
}


ControlAction::ControlAction(uint32_t deiId, uint32_t controlword):BaseAction(deiId,0), controlWord(controlword)
{
    actionType = BaseAction::ActionType::Control;
}

ControlAction::~ControlAction()
{

}
record_t& ControlAction::toPacket()
{
    data[INSTRUCTION_BYTE] = AUX::makeInstructionByte(dei, 0, Instructions::APPLY_CONTROL_WORD, 0);
    AUX::split(controlWord, data[CONTROL_BYTE0], data[CONTROL_BYTE1]);
    AUX::log(data,"Control Action.");
    return data;
}


void ControlAction::init(uint32_t dei, uint32_t ch, uint32_t trancive, uint32_t instr, uint32_t arincdata, float rt)
{

}
void  ControlAction::toData() {

}


BaseAction* MakeDataAction(uint32_t dei, uint32_t deichanell, uint32_t trancive, uint32_t instr, uint32_t arincdata, float rt){
    return new DataAction( dei,  deichanell,  trancive, instr, arincdata, rt);
}

BaseAction* MakeControlAction(uint32_t dei, uint32_t controlword){
    return new ControlAction( dei,  controlword);
}



