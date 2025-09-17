
#ifdef RASBERRY
#include "DEI1016RasberryConfigurations.h"

std::bitset<WORDSIZE> operator+ (std::bitset<WORDSIZE> b1, std::bitset<WORDSIZE> b2){
    std::bitset<WORDSIZE> b3(0);
    for(int i=0; i < WORDSIZE; i++){
       b3[i] = b1[i] || b2[i];
    }
    return b3;
 }
#endif
