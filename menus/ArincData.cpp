#include "ArincData.h"
#include "arinc_functions.h"
#include <cmath>
#include <string>
#include "utils.h"
/*
 *
 */

template <>
 value_t  DArincData::Get<LabelIdOctal>() const {
    uint32_t n2 = GetBit(1) * 2 + GetBit(2);
    uint32_t n1 = GetBit(3) * 4 + GetBit(4) * 2 + GetBit(5);
    uint32_t n0 = GetBit(6) * 4 + GetBit(7) * 2 + GetBit(8);
   // uint32_t s = n2 * 100 + n1 *10 +n0;

    std::string s = std::to_string(n2) + std::to_string(n1) + std::to_string(n0);

    return value_t(s.c_str());
}

template <>
void  DArincData::Set<LabelIdOctal>(const value_t& value) {
    // a translation from octal must be done here
    std::string _value = value.toString().toStdString();
    //
    uint32_t n2 = std::stoi(std::string{_value[0]}); // most  significant digit
    uint32_t n1 = std::stoi(std::string{_value[1]});
    uint32_t n0 = std::stoi(std::string{_value[2]}); // least significant digit
    //

    std::bitset<3> n0bits(n0);
    std::bitset<3> n1bits(n1);
    std::bitset<3> n2bits(n2);

    SetBit(1, n2bits[1]);
    SetBit(2, n2bits[0]);

    SetBit(3, n1bits[2]);
    SetBit(4, n1bits[1]);
    SetBit(5, n1bits[0]);

    SetBit(6, n0bits[2]);
    SetBit(7, n0bits[1]);
    SetBit(8, n0bits[0]);
}

template <>
value_t  DArincData::Get<LabelIdBits>() const {

    return ArincFunctions::BITS::Value<value_t>(1,LABEL_SIZE,*this);
}

template <>
void  DArincData::Set<LabelIdBits>(const value_t& value) {
    if (value.toString().size() == LABEL_SIZE){
        ArincFunctions::BITS::SetValue<value_t>(value,1,LABEL_SIZE,*this);
    }
}

template <>
value_t  DArincData::Get<Bits>(uint8_t& bi, uint8_t& bf) const {
    return ArincFunctions::BITS::Value<value_t>(bi, bf, *this);
}

template <>
value_t DArincData::Get<DataHex>() const {
    int n = ArincFunctions::INT::Value<uint32_t>(SDI_LSB, 32, *this);
    return value_t(str_t::number(n,16));
}

template <>
value_t  DArincData::Get<DataBits>() const {
    return ArincFunctions::BITS::Value<value_t>(SDI_LSB, ARINC32_SIZE, *this);
}

template <>
void DArincData::Set<DataBits>(const value_t& value){
    if ( value.toString().size() == ARINC32_SIZE-LABEL_SIZE){
        ArincFunctions::BITS::SetValue<value_t>(value, SDI_LSB, ARINC32_SIZE, *this);
    }
}


template <>
void DArincData::Set<Bits>(const value_t& value, const uint8_t& bi, const uint8_t& bf){
    if ( value.toString().size() == bf-bi+1){
        ArincFunctions::BITS::SetValue<value_t>(value, bi, bf, *this);
    }
}

template <>
void DArincData::Set<Bits>(const value_t& value){
        ArincFunctions::BITS::SetValue<value_t>(value, 1, ARINC32_SIZE, *this);
}

template <>
value_t  DArincData::Get<Bits>() const {
    return ArincFunctions::BITS::Value<value_t>(1, ARINC32_SIZE, *this);
}

template <>
value_t  DArincData::Get<int>() const {
    return ArincFunctions::INT::Value<value_t>(1, ARINC32_SIZE, *this);
}




