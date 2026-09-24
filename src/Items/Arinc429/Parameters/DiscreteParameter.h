#pragma once

#include "BaseParameter.h"
#include "types.h"

template <>
class Param<DArincParamType::DISCRETE> : public BaseParameter {
    public:
        Param(str_t json_file);
        Param(jsonobj_t json);
        void parseFromJson(jsonobj_t json) override;
        virtual value_t getEngValue(const DArincData& data) const override;
        virtual void setEngValue(const value_t& value, DArincData& data) override;
        virtual str_t getInfo() override;

};