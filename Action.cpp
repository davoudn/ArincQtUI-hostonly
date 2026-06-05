#include "action.h"
#include "bitutils.h"
#include <bitset>


BaseAction::BaseAction(uint32_t dei_id, uint32_t ch):dei(dei_id), channel(ch)
{
    for(int i=0; i < POCKET_SIZE; i++)
    {
        data[i]=static_cast<char>(0);
    }
}

BaseAction::~BaseAction(){

}


DataAction::DataAction(uint32_t dei, uint32_t ch, uint32_t tran_receive, uint32_t instr, uint32_t arinc_data, float rt):BaseAction(dei, ch),
    arinc_data(arinc_data), rate(rt), tran_receive(tran_receive), instruction(instr)
{
    action_type = BaseAction::ActionType::Data;
}

char* DataAction::toPacket()
{
    dword_t a{arinc_data};
    AUX::convertFromArincToDEI(a);
    uint32_t x = static_cast<uint32_t>(a.to_ulong());
    data[INITIAL_BYTE_INDEX] = INITIAL_BYTE;
    data[INSTRUCTION_BYTE] = AUX::makeInstructionByte(dei, channel, instruction, tran_receive);
    data[RATE_BYTE] = AUX::timeToBits(rate);
    AUX::split(x, data[ARINC_BYTE0], data[ARINC_BYTE1], data[ARINC_BYTE2], data[ARINC_BYTE3]);
    return data;
}


ControlAction::ControlAction(uint32_t dei_id, uint32_t control_word):BaseAction(dei_id,0), control_word(control_word)
{
    action_type = BaseAction::ActionType::Control;
}

char* ControlAction::toPacket()
{
    data[INSTRUCTION_BYTE] = AUX::makeInstructionByte(dei, 0, Instructions::APPLY_CONTROL_WORD, 0);
    AUX::split(control_word, data[CONTROL_BYTE0], data[CONTROL_BYTE1]);
    return data;
}


BaseAction* MakeDataAction(uint32_t dei, uint32_t channel, uint32_t tran_receive, uint32_t instr, uint32_t arinc_data, float rt){
    return new DataAction( dei,  channel,  tran_receive, instr, arinc_data, rt);
}

BaseAction* MakeControlAction(uint32_t dei, uint32_t control_word){
    return new ControlAction( dei,  control_word);
}



action::action(uint32_t ch, uint32_t trans_rec, uint32_t instr,
                                             uint32_t arinc_data, float rt, uint16_t control)
{
    setData(ch,  trans_rec,  instr,  arinc_data,  rt,  control);
}

void action::setData(uint32_t ch, uint32_t trans_receive, uint32_t instr,
                                                               uint32_t arinc_data, float rt, uint16_t control)
{
    channel = ch;
    tran_receive = trans_receive;
    instruction = instr;
    this->arinc_data   = arinc_data;
    rate        = rt;
    control_word = control;
}

char* action::toPacket()
{
    dword_t a{arinc_data};
    AUX::convertFromArincToDEI(a);
    uint32_t x = static_cast<uint32_t>(a.to_ulong());
data[INSTRUCTION_BYTE] = AUX::makeInstructionByte(0,channel, instruction, tran_receive);
    data[RATE_BYTE] = AUX::timeToBits(rate);
    AUX::split(x, data[ARINC_BYTE0], data[ARINC_BYTE1], data[ARINC_BYTE2], data[ARINC_BYTE3]);
    uint16_t ctrl_word = 0;
    AUX::split(control_word, data[CONTROL_BYTE0], data[CONTROL_BYTE1]);
    return data;
}