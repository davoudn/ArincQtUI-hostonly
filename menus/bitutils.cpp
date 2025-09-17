#include "bitutils.h"
#include <array>
#include "types.h"


namespace AUX {

const std::array<uint8_t,ARINC32_SIZE> arinc_DEI_wordsMap = { 7, 6, 5, 4 , 3, 2, 1, 0, 31, 30, 29, 8 ,9 , 10, 11, 12, 13, 14, 15, 16 , 17, 18, 19, 20, 21, 22, 23, 24 ,25 , 26, 27, 28};


void convertFromArincToDEI(dword_t& data){
    dword_t x{data} ;
    for (int i=0; data.size();i++){
        x[i] = data[arinc_DEI_wordsMap[i]];
    }
}

void convertFromDEIToArinc(dword_t& data){
    dword_t x{data} ;
    for (uint32_t i=0; i < data.size();i++){
        data[arinc_DEI_wordsMap[i]] = x[i];
    }
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
