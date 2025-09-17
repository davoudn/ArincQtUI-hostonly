

#pragma once
#include <stdint.h>
#include "types.h"

#define Byte0 0
#define Byte1 1
#define Byte2 2
#define Byte3 3
#define Byte4 4
#define Byte5 5
#define Byte6 6
#define Byte7 7

#define MAX_INTERVAL_TIMESCALE 1000.0
#define MAX_UINT8_VALUE 256


namespace AUX
{

template <typename T,typename U>
void split(T& t, U& u1, U& u2)
{
    u1 = *((volatile U*)&t);
    u2 = *((volatile U*)&t + 1);

}

template <typename T,typename U>
void split(T& t, U& u0, U& u1, U& u2, U& u3)
{
    u0 = *((volatile U*)&t);
    u1 = *((volatile U*)&t + 1);
    u2 = *((volatile U*)&t + 2);
    u3 = *((volatile U*)&t + 3);
}



template <typename U,typename T>
void merge(U& u0, U& u1, U& u2, U& u3, T& t)
{
    *((volatile U*)&t) = u0;
    *((volatile U*)&t + 1) = u1;
    *((volatile U*)&t + 2) = u2;
    *((volatile U*)&t + 3) = u3;
}


template <typename U,typename T>
void merge(U& u1, U& u2, T& t)
{
    *((volatile U*)&t) = u1;
    *((volatile U*)&t + 1) = u2;
}

void convertBytesToData(char* recData, uint8_t& chanel, float& rate, dword_t& arincData);
void convertDataToBytes(uint8_t& chanel, float& rate, dword_t& arincData, char* recData);

void convertFromDEIToArinc(dword_t& data, dword_t& x);
void convertFromArincToDEI(dword_t& data);
void convertFromDEIToArinc(dword_t& data);

inline uint8_t timeToBits(float& time) {
    return static_cast<char> (MAX_UINT8_VALUE * time / MAX_INTERVAL_TIMESCALE);
}
inline float bitsToTime(char& bits) {
    return MAX_INTERVAL_TIMESCALE * static_cast<uint8_t>(bits) / MAX_UINT8_VALUE;
}


} // end of name space AUX


