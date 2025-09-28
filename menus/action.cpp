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
char* DataAction::toPacket()
{
    dword_t a{arincData};
    AUX::convertFromArincToDEI(a);
    uint32_t x = static_cast<uint32_t>(a.to_ulong());
    data[INITIAL_BYTE_INDEX] = INITIAL_BYTE;
    data[FINAL_BYTE_INDEX] = FINAL_BYTE;
    data[INSTRUCTION_BYTE] = AUX::makeInstructionByte(dei, chanel, instruction, tranReceive);
    data[RATE_BYTE] = AUX::timeToBits(rate);
    AUX::split(x, data[ARINC_BYTE0], data[ARINC_BYTE1], data[ARINC_BYTE2], data[ARINC_BYTE3]);
    qInfo() << "\t"<<static_cast<uint8_t>(data[0]) <<"\t" << static_cast<uint8_t>(data[1])
            << "\t"<<static_cast<uint8_t>(data[2]) <<"\t" << static_cast<uint8_t>(data[3])
            << "\t"<<static_cast<uint8_t>(data[4]) <<"\t" << static_cast<uint8_t>(data[5])
            << "\t"<<static_cast<uint8_t>(data[6]) <<"\t" << static_cast<uint8_t>(data[7])
            << "\t" << static_cast<uint8_t>(data[8])<< "\t" << static_cast<uint8_t>(data[9]);
    return data;
}


ControlAction::ControlAction(uint32_t deiId, uint32_t controlword):BaseAction(deiId,0), controlWord(controlword)
{
    actionType = BaseAction::ActionType::Control;
}

ControlAction::~ControlAction()
{

}
char* ControlAction::toPacket()
{
    data[INSTRUCTION_BYTE] = AUX::makeInstructionByte(dei, 0, Instructions::APPLY_CONTROL_WORD, 0);
    AUX::split(controlWord, data[CONTROL_BYTE0], data[CONTROL_BYTE1]);
    return data;
}


void  ControlAction::init(char* bytes)
{

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
    // qInfo() << "**Arinc --> "<<std::bitset<32>(arincData).to_string( );
    dword_t a{arincData};
    AUX::convertFromArincToDEI(a);
   // qInfo() << "**DEI   --> "<<a.to_string( );
    uint32_t x = static_cast<uint32_t>(a.to_ulong());
    data[INSTRUCTION_BYTE] = AUX::makeInstructionByte(0,chanel, instruction, tranReceive);
    data[RATE_BYTE] = AUX::timeToBits(rate);
    AUX::split(x, data[ARINC_BYTE0], data[ARINC_BYTE1], data[ARINC_BYTE2], data[ARINC_BYTE3]);
    uint16_t controlWord = 0;
    AUX::split(controlWord, data[CONTROL_BYTE0], data[CONTROL_BYTE1]);

       qInfo() << "\t"<<static_cast<int>(data[0]) <<"\t" << static_cast<int>(data[1])
                    << "\t"<<static_cast<int>(data[2]) <<"\t" << static_cast<int>(data[3])
                    << "\t"<<static_cast<int>(data[4]) <<"\t" << static_cast<int>(data[5])
                    << "\t"<<static_cast<int>(data[6]) <<"\t" << static_cast<int>(data[7]) ;
    return data;
}
