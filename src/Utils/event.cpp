#include "event.h"
#include "bitutils.h"
#include <bitset>


BaseEvent::BaseEvent(uint32_t dei_id, uint32_t ch):dei(dei_id), channel(ch)
{
    for(int i=0; i < POCKET_SIZE; i++)
    {
        data[i]=static_cast<char>(0);
    }
}

BaseEvent::~BaseEvent(){

}


DataEvent::DataEvent(uint32_t dei, uint32_t ch, uint32_t tran_receive, uint32_t instr, uint32_t arinc_data, float rt):BaseEvent(dei, ch),
    arinc_data(arinc_data), rate(rt), tran_receive(tran_receive), instruction(instr)
{
    event_type = BaseEvent::EventType::Data;
}

char* DataEvent::toPacket()
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


ControlEvent::ControlEvent(uint32_t dei_id, uint32_t control_word):BaseEvent(dei_id,0), control_word(control_word)
{
    event_type = BaseEvent::EventType::Control;
}

char* ControlEvent::toPacket()
{
    data[INSTRUCTION_BYTE] = AUX::makeInstructionByte(dei, 0, Instructions::APPLY_CONTROL_WORD, 0);
    AUX::split(control_word, data[CONTROL_BYTE0], data[CONTROL_BYTE1]);
    return data;
}


BaseEvent* MakeDataEvent(uint32_t dei, uint32_t channel, uint32_t tran_receive, uint32_t instr, uint32_t arinc_data, float rt){
    return new DataEvent( dei,  channel,  tran_receive, instr, arinc_data, rt);
}

BaseEvent* MakeControlEvent(uint32_t dei, uint32_t control_word){
    return new ControlEvent( dei,  control_word);
}
