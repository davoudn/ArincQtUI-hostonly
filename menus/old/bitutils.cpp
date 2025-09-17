#include "bitutils.h"
#include <array>
#include "types.h"


namespace AUX {

 std::array<uint8_t,ARINC32_SIZE> arinc_DEI_WordsMap;



void convertFromArincToDEI(dword_t& data){
    dword_t x ;
    for (int i=0; data.size();i++){
        x[i] = data[arinc_DEI_WordsMap[i]];
    }
    data = x;
}

void convertFromDEIToArinc(dword_t& data){
    dword_t x ;
    for (int i=0; data.size();i++){
        data[arinc_DEI_WordsMap[i]] = x[i];
    }
    data = x;
}

void convertBytesToData(char* recData, uint8_t& chanel, float& rate, std::bitset<ARINC32_SIZE>& arincData){
    chanel = recData[Byte0];
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
