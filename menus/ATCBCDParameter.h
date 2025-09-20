#ifndef ATCBCDPARAMETER_H
#define ATCBCDPARAMETER_H

#include "BaseParameter.h"
#include "types.h"

template <>
class Param<DArincParamType::ATCBCD> : public BaseParameter {
public:
    Param() = delete;
    Param(jsonobj_t _json) ;
    void ParseFromJson(jsonobj_t _json) override;
    virtual value_t EngValue(const DArincData& Data) override;
    virtual void setEngValue(const value_t& value, DArincData& data)  override;

    virtual  str_t getInfo() override;

protected:
    uint8_t SigBits = 0;

};

#endif // ATCBCDPARAMETER_H
