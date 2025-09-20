#pragma once

#include "ArincData.h"
#include "types.h"
#include "baseitem.h"

#include <map>
#include <array>



class DArincData;

struct ScalePlus {};
struct ScaleMinus{};
struct Resolution {};

/*
 *  A general Arinc parameter class which could handle all sort of data, it has a name, initial bit (IBit) and
 *   final bit (Fbit)  and a details str to str map.
 *    for bnr: details contains SigBit and Scale among others
 *    for bcd: details contains SigBit  among others
 *    for discrete : details contains discrete values each parameter may accept.
 */
 std::ostream& operator << (std::ostream& os, const jsonobj_t& json);

 // "Units": "Deg:Min", "Scale-": "180", "Scale+": "180", "SigBits": "6", "Resolution": "0.1", "MinTransit(msec)": "250", "MaxTransmit(msec)": "500"
#define NUM_BNRBCD_PARAMETER_NAMES 7
const std::array<str_t, NUM_BNRBCD_PARAMETER_NAMES> BNRBCD_PARAMETER_NAMES = {"Units", "Scale-", "Scale+", "SigBits", "Resolution", "MinTransit(msec)", "MaxTransmit(msec)"};

#define NUM_DICRETE_PARAMETER_NAMES 4
const std::array<str_t, NUM_DICRETE_PARAMETER_NAMES> DICRETE_PARAMETER_NAMES = {"IBit", "FBit", "Name", "Details"};

template <typename T> 
bool Assert(jsonobj_t json);

class BaseParameter:public BaseItem {
    public:
    BaseParameter(jsonobj_t _json) = delete;
    BaseParameter(BaseParameter&) = default;
    BaseParameter();
    virtual ~BaseParameter() = default;

    value_t BitsValue(const DArincData& data) ;
    void    setBitsValue(const value_t& value, DArincData& data) ;

    virtual value_t EngValue(const DArincData& data) = 0;
    virtual void setEngValue(const value_t& value, DArincData& data) = 0;

    
    virtual void ParseFromJson(jsonobj_t _json) = 0;

    virtual str_t GetName(){
       return Name;
    }
    inline const uint32_t& getType(){
        return Type;
    }
    inline const std::map<str_t, value_t>& GetDetails(){
         return Details;
    }
    virtual str_t getInfo() = 0;
     value_t getDetail(str_t _detailName);

protected:
    uint8_t  IBit = 0;
    uint8_t  FBit = 0;
    uint32_t Type = DArincParamType::UNKOWN;

    str_t Name;
    std::map<str_t, value_t> Details;
};

template <int P>
class Param : public BaseParameter{};

