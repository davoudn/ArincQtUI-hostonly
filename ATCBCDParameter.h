#pragma once

#include "BaseParameter.h"
#include "types.h"

template <>
class Param<DArincParamType::ATCBCD> : public BaseParameter 
{
public:
    Param() = delete;
    Param(jsonobj_t json);
    Param(const str_t& name, int i_bit, int f_bit
        , const str_t& details);
    void parseFromJson(jsonobj_t json) override;
    virtual value_t getEngValue(const DArincData& data) const override;
    virtual str_t getInfo() const override;
    virtual void setEngValue(const value_t& value
        , DArincData& data) const override;
protected:
    uint8_t sig_bits = 0;

};
