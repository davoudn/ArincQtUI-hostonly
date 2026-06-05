#ifndef INTPARAMETER_H
#define INTPARAMETER_H
#pragma once

#include "BaseParameter.h"
#include "types.h"

template <>
class Param<DArincParamType::INT> : public BaseParameter {
    public:
        Param() = delete;
        Param(jsonobj_t json);
        Param(const str_t& name, int i_bit, int f_bit, const str_t& details);
        void parseFromJson(jsonobj_t json) override;
        virtual value_t engValue(const DArincData& data) override;
        virtual void setEngValue(const value_t& value, DArincData& data) override;
        virtual str_t getInfo() override;
};

#endif // INTPARAMETER_H
