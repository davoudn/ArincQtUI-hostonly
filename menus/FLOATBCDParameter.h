#ifndef FLOATBCDPARAMETER_H
#define FLOATBCDPARAMETER_H
#include "types.h"
#include "arinc_functions.h"
#include "BaseParameter.h"

template<>
class Param<DArincParamType::FLOATBCD>: public BaseParameter
{
public:
    Param(jsonobj_t _json) ;
    void ParseFromJson(jsonobj_t _json) override;
    virtual value_t EngValue(const DArincData& Data) override;
    virtual void setEngValue(const value_t& value, DArincData& data)  override;
    virtual str_t getInfo() override;
private:
    uint8_t SigBits = 0;
};

#endif // FLOATBCDPARAMETER_H
