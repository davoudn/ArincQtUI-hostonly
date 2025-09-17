#include "arinc_functions.h"
#include "math.h"
#include "ArincData.h"
#include "types.h"
#include "utils.h"

#include <cstdint>
#include <algorithm>
#include <vector>
#include <map>

namespace  ArincFunctions {
using pair_t = std::pair<uint32_t,uint32_t>;

constexpr  std::array<pair_t, 5> BCDSplit  = {pair_t{27,29} , pair_t{23,26}, pair_t{19,22},  pair_t{15,18}, pair_t{11,14}};

// got from ARINC SPECIFICATION 429, PART 1 - Page 89
constexpr  std::array<pair_t, 6> BCDSplit6 = {pair_t{29,29} , pair_t{25,28}, pair_t{21,24},  pair_t{17,20}, pair_t{13,16}, pair_t{9,12}};

// got from ARINC SPECIFICATION 429, PART 1 - Page 80
constexpr  std::array<pair_t, 4> BCDFLNO   = {pair_t{26,29} , pair_t{22,25}, pair_t{18,21},  pair_t{14,17}};

// got from ARINC SPECIFICATION 429, PART 1 - Page 110
constexpr  std::array<pair_t, 4> BCDATC    = {pair_t{27,29} , pair_t{24,26}, pair_t{21,23},  pair_t{18,20}};

const std::map<str_t, DArincParamType> MAP_PARAM_TYPE = { {"BNR", DArincParamType::BNR}, {"BCD", DArincParamType::BCD},
                                                   {"DISCRETE", DArincParamType::DISCRETE} };

void AddOneAt(uint32_t i, DArincData& Data){
    if (Data.GetBit(i)==1) {
        Data.SetBit(i,0);
        if (i+1<BCDBNR_MSB-1){
            AddOneAt(i+1,Data);
        }
    }
    if (Data.GetBit(i)==0){
        Data.SetBit(i,1);
    }
}

void TwosComplement(uint32_t bi, DArincData& Data){
    //
    for(int i=bi; i<BCDBNR_MSB; i++){
        if (Data.GetBit(i)==1) {
            Data.SetBit(i,0);
        }
        else {
            Data.SetBit(i,1);
        }
    }
}

/*
BITS
*/
template<>
value_t BITS::Value<value_t>(const uint8_t& bi, const uint8_t& bf, const DArincData& Data){
    str_t t;
    for (uint8_t f = bi; f <= bf; f++) {
        t += str_t::number(Data.GetBit(f));
    }
    return value_t(t);
}

template<>
void   BITS::SetValue<value_t>(const value_t& value, const uint8_t& bi, const uint8_t& bf, DArincData& Data){
    // some assert that string only consists of 0 and 1 charachter may be a good idea
    std::vector<uint8_t> bits;
    for (auto& x: value.toString()){
        if (x!='0' && x!='1'){
            return;
        }
    }
    if (value.toString().size() != bf-bi+1) {
        return;
    }

    for (auto& x: value.toString()){
        if (x=='0'){
            bits.push_back(0);
        }
        if (x=='1'){
            bits.push_back(1);
        }
    }

    for (int i=0; i < bits.size(); i++){
        Data.SetBit(i+bi,bits[i]);

    }
}

/*
        HEX
*/
template<>
uint32_t HEX::Value<uint32_t>      (const uint8_t& bi, const uint8_t& bf, const DArincData& Data){
    uint32_t t{ 0 };
    return t;
}

template<>
value_t HEX::Value<value_t>(const uint8_t& bi, const uint8_t& bf, const DArincData& Data){
    return value_t(Value<uint32_t>(bi, bf, Data));
}


/*
INT
*/

template<>
uint32_t INT::Value<uint32_t>(const uint8_t& bi, const uint8_t& bf, const DArincData& Data){
                 uint32_t d{ 0 };
                 for (uint8_t f = bi; f <= bf; f++) {
                     d += Data.GetBit(f) * std::pow(2, f - bi);
                 }
                 return d;
}

template<>
value_t INT::Value<value_t>(const uint8_t& bi, const uint8_t& bf, const DArincData& Data){
    return value_t(Value<uint32_t>(bi, bf, Data));
}

template <>
void INT::SetValue<value_t>(const value_t& t ,const uint8_t& bi, const uint8_t& bf,  DArincData& Data){
    auto bits = std::bitset<16>(t.toUInt());
    if (bf == bi+15){
       for (int i=0; i<16;i++){
         Data.SetBit(bi+i, bits[i]);
       }
    }
}

/*
 * Flight number BCD, similar BCD but with 4 bits MSDigits, with scale =9999
*/
template<>
uint32_t  FLNOBCD::Value<uint32_t>   (const DArincData& Data){
    double t{ 0.0 };
    int N = sigdigits - 1;
    for (uint8_t x = 0; x <= N; x++) {
            t += std::pow(10, N - x) * INT::Value<uint32_t>(BCDFLNO[x].first, BCDFLNO[x].second, Data);
    }
    return t;
}

template<>
value_t  FLNOBCD::Value<value_t>   (const DArincData& Data){
    return FLNOBCD::Value<uint32_t>  (Data);
}

template<>
void FLNOBCD::SetValue<value_t>  (const value_t& value, DArincData& Data){

    double _value = value.toDouble() * utils::sign(value.toDouble());
    auto decimals = utils::getDecimals(_value, sigdigits);
    // qDebug() << decimals << "<----decimals, sigdigits---->" << sigdigits<< "\n";

    if (0<sigdigits && sigdigits <= 5){
        for (int i=0; i < sigdigits; i++) {
            auto bits = std::bitset<4>(decimals[i]);
            for (uint8_t j= BCDFLNO[i].first; j <= BCDFLNO[i].second; j++){
                auto c = j - BCDFLNO[i].first;
                auto x = bits[c];
                Data.SetBit(j,x);
            }
        }
    }

}


/*
 * ATC BCD, similar BCD but with 3 bits Digits, with scale = 7777
*/
template<>
uint32_t  ATCBCD::Value<uint32_t>   (const DArincData& Data){
    double t{ 0.0 };
    int N = sigdigits - 1;
    for (uint8_t x = 0; x <= N; x++) {
        t += std::pow(10, N - x) * INT::Value<uint32_t>(BCDATC[x].first, BCDATC[x].second, Data);
    }
    return t;
}

template<>
value_t  ATCBCD::Value<value_t>   (const DArincData& Data){
    return ATCBCD::Value<uint32_t>  (Data);
}

template<>
void ATCBCD::SetValue<value_t>  (const value_t& value, DArincData& Data){

    double _value = value.toDouble() * utils::sign(value.toDouble());
    auto decimals = utils::getDecimals(_value, sigdigits);
    // qDebug() << decimals << "<----decimals, sigdigits---->" << sigdigits<< "\n";

    if (0<sigdigits && sigdigits <= 4){
        for (int i=0; i < sigdigits; i++) {
            auto bits = std::bitset<4>(decimals[i]);
            for (uint8_t j= BCDATC[i].first; j <= BCDATC[i].second; j++){
                auto c = j - BCDATC[i].first;
                auto x = bits[c];
                Data.SetBit(j,x);
            }
        }
    }

}

/*
BCD
*/

template<>
uint32_t    BCD::Value<uint32_t>   (const uint8_t& sigdigits, const DArincData& Data){
            double t{ 0.0 };
            int N = sigdigits - 1;
            if (0<sigdigits && sigdigits <= 5){
                for (uint8_t x = 0; x <= N; x++) {
                    t += std::pow(10, N - x) * INT::Value<uint32_t>(BCDSplit[x].first, BCDSplit[x].second, Data);
                }
            }
            if (sigdigits == 6){
                    for (uint8_t x = 0; x <= N; x++) {
                        t += std::pow(10, N - x) * INT::Value<uint32_t>(BCDSplit6[x].first, BCDSplit6[x].second, Data);
                    }

            }

            return t;
}

template<>
void BCD::SetValue<value_t>  (const value_t& value, const uint8_t& sigdigits, DArincData& Data){

    double _value = value.toDouble() * utils::sign(value.toDouble());
    auto decimals = utils::getDecimals(_value, sigdigits);

    if (0<sigdigits && sigdigits <= 5){
        for (int i=0; i < sigdigits; i++) {
            auto bits = std::bitset<4>(decimals[i]);
            for (uint8_t j= BCDSplit[i].first; j <= BCDSplit[i].second; j++){
              auto c = j - BCDSplit[i].first;
              auto x = bits[c];
              Data.SetBit(j,x);
            }
        }
    }

    if (0<sigdigits && sigdigits == 6){
        for (int i=0; i < sigdigits; i++) {
            auto bits = std::bitset<4>(decimals[i]);
            for (uint8_t j= BCDSplit6[i].first; j <= BCDSplit6[i].second; j++){
                auto c = j - BCDSplit6[i].first;
                auto x = bits[c];
                Data.SetBit(j,x);
            }
        }
    }

    if (utils::sign(value.toDouble())<0) {
        Data.SetBit(SSM_LSB,1);
        Data.SetBit(SSM_MSB,1);
    }
    else {
        Data.SetBit(SSM_LSB,0);
        Data.SetBit(SSM_MSB,0);
    }
}

template<>
value_t BCD::Value<value_t> (const uint8_t& sigdigits, const DArincData& Data){
            return value_t(BCD::Value<uint32_t>(sigdigits, Data));
}

double BCD::getMax(uint32_t sigdigits){
    DArincData Data;
    double tmp{0.0};

    std::vector<uint8_t> maxdecimals;
    if (sigdigits<6){
        for (uint32_t i=0;i<sigdigits-1;i++){
            maxdecimals.push_back(9);
        }
        maxdecimals.push_back(7);

        tmp=0.0;
        for (uint8_t x = 0; x < sigdigits; x++) {
            tmp+=maxdecimals[x] * std::pow(10,x);
        }
    }
    //
    if (sigdigits==6){
        for (uint32_t i=0;i<sigdigits-1;i++){
            maxdecimals.push_back(9);
        }
        maxdecimals.push_back(1);

        tmp=0.0;
        for (uint8_t x = 0; x < sigdigits; x++) {
            tmp+=maxdecimals[x] * std::pow(10,x);
        }
    }
    return tmp;
}

/*
FLOATBCD: similar to bcd but  MSDECIMAL*10 + (MSDECIMAL-1)*10^-1 + ....

mostly designed for radio, ARINC SPECIFICATION 429, PART 1 - Page 109
*/

template<>
uint32_t    FLOATBCD::Value<uint32_t>   (const uint8_t& sigdigits, const DArincData& Data){
    double t{ 0.0 };
    int N = 1;
    if (0<sigdigits && sigdigits <= 5){
        for (uint8_t x = 0; x <= N; x++) {
            t += std::pow(10, N - x) * INT::Value<uint32_t>(BCDSplit[x].first, BCDSplit[x].second, Data);
        }
    }
    return t;
}

template<>
void FLOATBCD::SetValue<value_t>  (const value_t& value, const uint8_t& sigdigits, DArincData& Data){

    double _value = value.toDouble() * std::pow(10 ,sigdigits-1);
    auto decimals = utils::getDecimals(_value, sigdigits);
    // qDebug() << decimals << "<----decimals, sigdigits---->" << sigdigits<< "\n";

    if (0<sigdigits && sigdigits <= 5){
        for (int i=0; i < sigdigits; i++) {
            auto bits = std::bitset<4>(decimals[i]);
            for (uint8_t j= BCDSplit[i].first; j <= BCDSplit[i].second; j++){
                auto c = j - BCDSplit[i].first;
                auto x = bits[c];
                Data.SetBit(j,x);
            }
        }
    }

        Data.SetBit(SSM_LSB,0);
        Data.SetBit(SSM_MSB,0);
}

template<>
value_t FLOATBCD::Value<value_t> (const uint8_t& sigdigits, const DArincData& Data){
    return value_t(FLOATBCD::Value<uint32_t>(sigdigits, Data));
}

double FLOATBCD::getMax(uint32_t sigdigits){
    DArincData Data;
    double tmp{0.0};

    std::vector<uint8_t> maxdecimals;
    if (sigdigits<6){
        for (uint32_t i=0;i<sigdigits-1;i++){
            maxdecimals.push_back(9);
        }
        maxdecimals.push_back(7);

        tmp=0.0;
        for (uint8_t x = 0; x < sigdigits; x++) {
            tmp+=maxdecimals[x] * std::pow(10,x);
        }
    }
    //
    if (sigdigits==6){
        for (uint32_t i=0;i<sigdigits-1;i++){
            maxdecimals.push_back(9);
        }
        maxdecimals.push_back(1);

        tmp=0.0;
        for (uint8_t x = 0; x < sigdigits; x++) {
            tmp+=maxdecimals[x] * std::pow(10,x);
        }
    }
    return tmp;
}


/*
BNR
*/
template<>
double BNR::Value<double> (const uint8_t& bi, const uint8_t& bf, const DArincData& Data){
    double t{ 0.0 };
    if (Data.GetBit(bf)==0) {
        for (uint8_t f = bf - 1; f >= bi; f--) {
            t += std::pow(0.5, bf-f) * (double)Data.GetBit(f);
        }
    }
    else {
        for (uint8_t f = bf - 1; f >= bi; f--) {
            t += std::pow(0.5, bf-f) * (double)Data.GetBit(f);
        }
        t = 1.0 - t;
    }
    return t;
}

template<>
value_t BNR::Value<value_t> (const uint8_t& bi, const uint8_t& bf, const DArincData& Data){
            return value_t(Value<double>(bi, bf, Data));
}


template <>
void  BNR::SetValue<double>(const double& t ,const uint8_t& bi, const uint8_t& bf,  DArincData& Data){
        double __t{0.f};
        double _t{std::abs(t)};
        for (int f=bf-1 ; f >= bi; f--){
            __t = 2 * _t;
            Data.SetBit(f,(uint8_t) __t);
            __t -= (uint8_t) __t;
            _t = __t;
        }
        if (t<0.0){
            TwosComplement(bi,Data);
            AddOneAt(bi,Data);
            Data.SetBit(bf,1);
        }
}

template <>
void  BNR::SetValue<value_t>(const value_t& t ,const uint8_t& bi, const uint8_t& bf,  DArincData& Data){
    SetValue<double>(t.toDouble() , bi, bf, Data);
}


/*
 *  IEEE FLOATING POINT FORMAT: N=C*B^Q*(-1)^S
 *  C= MANTISSA
 *  Q= EXPONENT
 *  S = SIGN
 */

template <>
int IEEEFLOT::Value<int>(const uint8_t& bi, const uint8_t& exponent, const uint8_t& mantissa,const DArincData& Data){
    uint32_t q = INT::Value<uint32_t>(bi,bi-exponent+1,Data);
    uint32_t c = INT::Value<uint32_t>(bi-exponent, bi-exponent-mantissa+1, Data);
    uint32_t s = Data.GetBit(27);
    return c * std::pow(10,q) * std::pow(-1,s);
}

template <>
value_t IEEEFLOT::Value<value_t>(const uint8_t& bi, const uint8_t& exponent, const uint8_t& mantissa,const DArincData& Data){
    return IEEEFLOT::Value<int>(bi, exponent, mantissa, Data);
}



template <>
void IEEEFLOT::SetValue<value_t>(const value_t& t, const uint8_t& bi, const uint8_t& exponent, const uint8_t& mantisa, DArincData& Data){

}


} // end of namespace
