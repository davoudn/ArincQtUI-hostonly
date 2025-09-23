#include "bitutils.h"
#include <array>
#include "types.h"


namespace AUX {

const std::array<uint8_t,ARINC32_SIZE> arinc_DEI_wordsMap = { 7, 6, 5, 4 , 3, 2, 1, 0, 31, 30, 29, 8 ,9 , 10, 11, 12, 13, 14, 15, 16 , 17, 18, 19, 20, 21, 22, 23, 24 ,25 , 26, 27, 28};

uint16_t getInstruction(std::bitset<8>& x)
{
    return x[0] + x[1] * 2 + x[2] * 4 + x[3] * 8;
}

uint16_t getChanel(std::bitset<8>& x) {
    return x[5] + x[6] * 2;
}

uint16_t getTransReceive(std::bitset<8>& x) {
    return x[4];
}

uint8_t makeInstructionByte(uint32_t ch, uint32_t instr, uint32_t transrec){
    std::bitset<8> insByte;
    AUX::setChanel(insByte, ch);
    AUX::setTransReceive(insByte, transrec);
    AUX::setInstruction(insByte, instr);
    return static_cast<uint8_t>(insByte.to_ulong());
}

void setInstruction(std::bitset<8>& x, uint32_t ins){
    std::bitset<4> insbits{ins};
    x[0] = insbits[0];
    x[1] = insbits[1];
    x[2] = insbits[2];
    x[3] = insbits[3];
}
void setChanel(std::bitset<8>& x, uint32_t ch){
    std::bitset<2> insbits{ch};
    x[5] = insbits[0];
    x[6] = insbits[1];
}
void setTransReceive(std::bitset<8>& x, uint32_t tranceive){
    x[4] = tranceive;
}

void convertFromArincToDEI(dword_t& data){
    dword_t x{data} ;
    for (int i=0; i<data.size();i++){
      data[i] = x[arinc_DEI_wordsMap[i]];
    }
}

void convertFromArincToDEI(uint32_t& data){
    dword_t databits{data};
    convertFromDEIToArinc(databits);
    data = static_cast<uint32_t>(databits.to_ulong());
}



void convertFromDEIToArinc(dword_t& data){
    dword_t x{data} ;
    for (uint32_t i=0; i < data.size();i++){
        data[arinc_DEI_wordsMap[i]] = x[i];
    }
}

void convertFromDEIToArinc(uint32_t& data){
    dword_t databits{data};
    convertFromDEIToArinc(databits);
    data = static_cast<uint32_t>(databits.to_ulong());
}


void convertFromDEIToArinc(dword_t& data, dword_t& x){
    for (uint32_t i=0; i < data.size();i++){
        x[arinc_DEI_wordsMap[i]] = data[i];
    }
}

void convertBytesToData(char* recData, uint8_t& chanel, float& rate, std::bitset<ARINC32_SIZE>& arincData){
    chanel = static_cast<uint8_t>(recData[Byte0]);
    rate = AUX::bitsToTime(recData[Byte1]);
    uint32_t dword = 0;
    AUX::merge(recData[Byte4],recData[Byte5],recData[Byte6],recData[Byte7],dword);
    arincData = std::bitset<ARINC32_SIZE>(dword);
}

void convertDataToBytes(uint8_t& chanel, float& rate, std::bitset<ARINC32_SIZE>& arincData, char* recData){
    recData[Byte0] = chanel;
    recData[Byte1] = timeToBits(rate);
    uint32_t dword = static_cast<uint32_t>(arincData.to_ulong());
    split(dword, recData[Byte4],recData[Byte5],recData[Byte6],recData[Byte7]);
}


}// end namespace AUX
