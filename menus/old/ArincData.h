#pragma once
#include <bitset>
#include <cstdlib>
#include <iostream>
#include <cstdint>

#include "types.h"

struct LabelIdBits {};
struct LabelIdOctal {};
struct DataHex {};
struct DataBits {};
struct RawData{};
struct Bits{};
/*
 *  
*/
class DArincData {
public:
    DArincData() {
    }

    DArincData(uint32_t _data):Data(_data) {
    }

 //
 template <typename T>
    void Init(T& t){
        Data = std::bitset<ARINC32_SIZE>(t);
    }

 template <typename T>
 inline   void Init(T&& t) {
        Data = std::bitset<ARINC32_SIZE>(t);
    }
 //
inline uint32_t Size() {
        return Data.size();
}

inline const std::bitset<ARINC32_SIZE>& getBitSet() {
    return Data;
}

template <typename T>
value_t Get() const ;

template <typename T>
value_t Get(uint8_t& bi, uint8_t& bf) const ;

template <typename T>
void Set(const value_t& value)  ;

template <typename T>
void Set(const value_t& value, const uint8_t& bi, const uint8_t& bf)  ;

  // get, set functions
 inline uint8_t GetBit(int n) const {
        if( n>0 && n<=ARINC32_SIZE){
          return Data[n - 1];
        }
        std::cout << "ArincData::GetValue n = " << n << " is out of range.\n";
        return 0;
 }

 inline void SetBit(uint32_t n, uint32_t val) {
        if( 0<n && n <= 32){
           Data[n-1] = val;
           return;
        }
        std::cout << "ArincData::SetBit n = "<<n<< " is out of range.\n";
 }

 //
 inline void UpdateData(std::bitset<ARINC32_SIZE>& _Data) {
        Data = _Data;
 }
 //
 inline void UpdateData(std::bitset<ARINC32_SIZE>&& _Data) {
        Data = _Data;
 }

protected:
    std::bitset<ARINC32_SIZE> Data;
};
