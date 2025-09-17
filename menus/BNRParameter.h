#include "BaseParameter.h"
#include "types.h"

template <>
class Param<DArincParamType::BNR> : public BaseParameter {
    public:
    Param(jsonobj_t _json) ;
    void ParseFromJson(jsonobj_t _json) override;
    virtual value_t EngValue(const DArincData& Data) override;
    virtual void setEngValue(const value_t& value, DArincData& data)  override;
    virtual str_t getInfo() override;
};
