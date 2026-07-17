#pragma once

#include "types.h"
#include "BaseParameter.h"

template <>
class Param<DArincParamType::FLNOBCD> : public BaseParameter {
public:
    Param() = delete;
    Param(jsonobj_t json);
    Param(const str_t& name, int i_bit, int f_bit, const str_t& details);
    void parseFromJson(jsonobj_t json) override;
    virtual value_t getEngValue(const DArincData& data) const override;
    virtual void setEngValue(const value_t& value, DArincData& data) override;

    virtual str_t getInfo() override;

protected:
    uint8_t sig_bits = 0;

};
