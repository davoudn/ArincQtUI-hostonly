#pragma once

#include "ArincData.h"
#include "types.h"
#include "baseitem.h"
#include <map>
#include <array>
#define NUM_BNRBCD_PARAMETER_NAMES 7
#define NUM_DICRETE_PARAMETER_NAMES 4
class DArincData;
struct ScalePlus {};
struct ScaleMinus{};
struct Resolution {};
std::ostream& operator << (std::ostream& os,
    const jsonobj_t& json);
const std::array<str_t, 
    NUM_BNRBCD_PARAMETER_NAMES> BNRBCD_PARAMETER_NAMES = 
    {"Units", "Scale-", "Scale+", "SigBits", 
        "Resolution", "MinTransit(msec)", "MaxTransmit(msec)"};
const std::array<str_t, NUM_DICRETE_PARAMETER_NAMES> 
    DICRETE_PARAMETER_NAMES = {"IBit", "FBit", "Name", "Details"};

template <typename T> 
bool Assert(jsonobj_t json);

class BaseParameter:public BaseItem {
    public:
        BaseParameter(jsonobj_t json) = delete;
        BaseParameter(BaseParameter&) = default;
        BaseParameter();
        BaseParameter(const str_t& name, int i_bit,
            int f_bit, const str_t& details);
        virtual ~BaseParameter() = default;
        virtual void parseFromJson(jsonobj_t json) = 0;
        virtual void setEngValue(const value_t& value, 
            DArincData& data) = 0;
        virtual str_t getInfo() = 0;
        virtual str_t getName();
        virtual value_t getEngValue(const DArincData& data) 
            const  = 0;
        void setBitsValue(const value_t& value, 
            DArincData& data);
        value_t getBitsValue(const DArincData& data) const;
        const uint32_t& getType();
        const std::map<str_t, value_t>& getDetails();
        value_t getDetail(str_t detail_name);
protected:
        uint8_t i_bit = 0;
        uint8_t f_bit = 0;
        uint32_t type = DArincParamType::UNKOWN;
        str_t name;
        std::map<str_t, value_t> details;
};

template <int P>
class Param : public BaseParameter
{
};