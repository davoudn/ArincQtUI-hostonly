#include "action.h"
#include "bitutils.h"
#include <bitset>

uint8_t makeInstructionByte(uint32_t ch, uint32_t transrec, uint32_t instr){
    std::bitset<8> insByte;
    AUX::setChanel(insByte, ch);
    AUX::setTransReceive(insByte,transrec);
    AUX::setInstruction(insByte, instr);
    return static_cast<uint8_t>(insByte.to_ulong());
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
std::array<uint8_t,TRANSMMIT_PACKET_SIZE>& action::toArrayPacket()
{
    AUX::convertFromArincToDEI(arincData);
    dataArray[INSTRUCTION_BYTE] = makeInstructionByte(chanel, tranReceive, instruction);
    dataArray[RATE_BYTE] = AUX::timeToBits(rate);
    AUX::split(arincData, dataArray[ARINC_BYTE0], dataArray[ARINC_BYTE1], dataArray[ARINC_BYTE2], dataArray[ARINC_BYTE3]);
    AUX::split(controlWord, dataArray[CONTROL_BYTE0], dataArray[CONTROL_BYTE1]);
    return dataArray;
}

char* action::toPacket()
{
    AUX::convertFromArincToDEI(arincData);
    data[INSTRUCTION_BYTE] = makeInstructionByte(chanel, tranReceive, instruction);
    data[RATE_BYTE] = AUX::timeToBits(rate);
    AUX::split(arincData, data[ARINC_BYTE0], data[ARINC_BYTE1], data[ARINC_BYTE2], data[ARINC_BYTE3]);
    AUX::split(controlWord, data[CONTROL_BYTE0], data[CONTROL_BYTE1]);
    return data;
}
