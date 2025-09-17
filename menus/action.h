#ifndef ACTION_H
#define ACTION_H
#include "types.h"
#include <stdint.h>
#include <array>

class action
{
public:
    action() = delete;
    action(uint32_t chanel, uint32_t trans_rec, uint32_t instr, value_t arincData, value_t rate, uint16_t control);

    bool bIfApplied = false;

    std::array<uint8_t,TRANSMMIT_PACKET_SIZE> dataArray;
    char data[TRANSMMIT_PACKET_SIZE];

    float rate = 0.f;
    uint16_t controlWord = 0;
    uint32_t arincData = 0;
    uint32_t instruction = 0;
    uint32_t chanel = 0;
    uint32_t tranReceive = 0;
    void setData(uint32_t chanel, uint32_t trans_rec, uint32_t instr, value_t arincData, value_t rate, uint16_t control);
    std::array<uint8_t,TRANSMMIT_PACKET_SIZE>& toArrayPacket();
    char* toPacket();


};

#endif // ACTION_H
