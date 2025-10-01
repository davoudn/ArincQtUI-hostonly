

#pragma once
#include <stdint.h>
#include <bitset>
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


template <typename T>
void log (T& data, str_t msg)
{
    if (data.size()== RX_BUFFER_SIZE)
    {
        qInfo() << msg;
        qInfo() << "\t"<<static_cast<uint8_t>(data[0]) <<"\t" << static_cast<uint8_t>(data[1])
                << "\t"<<static_cast<uint8_t>(data[2]) <<"\t" << static_cast<uint8_t>(data[3])
                << "\t"<<static_cast<uint8_t>(data[4]) <<"\t" << static_cast<uint8_t>(data[5])
                << "\t"<<static_cast<uint8_t>(data[6]) <<"\t" << static_cast<uint8_t>(data[7])
                << "\t"<<static_cast<uint8_t>(data[8]) << "\t"<< static_cast<uint8_t>(data[9]);
    }
}


uint16_t getInstruction(std::bitset<8>& x);
uint16_t getChanel(std::bitset<8>& x);
uint16_t getTransReceive(std::bitset<8>& x);
uint8_t makeInstructionByte(uint32_t dei,uint32_t ch, uint32_t instr, uint32_t transrec);

void setInstruction(std::bitset<8>& x, uint32_t ins);
void setChanel(std::bitset<8>& x, uint32_t ch);
void setTransReceive(std::bitset<8>& x, uint32_t tranceive);
void setDEI(std::bitset<8>& x, uint32_t dei);


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

void convertBytesToData(record_t& recData, uint8_t& dei, uint8_t& chanel, float& rate, std::bitset<ARINC32_SIZE>& arincData);

void convertFromDEIToArinc(dword_t& data, dword_t& x);
void convertFromArincToDEI(dword_t& data);
void convertFromDEIToArinc(dword_t& data);
void convertFromArincToDEI(uint32_t& data);
void convertFromDEIToArinc(uint32_t& data);

inline uint8_t timeToBits(float& time)
{
    return static_cast<char> (MAX_UINT8_VALUE * time / MAX_INTERVAL_TIMESCALE);
}
inline float bitsToTime(char bits)
{
    return MAX_INTERVAL_TIMESCALE * static_cast<uint8_t>(bits) / MAX_UINT8_VALUE;
}

} // end of name space AUX


