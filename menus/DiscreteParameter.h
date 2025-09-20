#include "BaseParameter.h"
#include "types.h"

#include <map>

template <>
class Param<DArincParamType::DISCRETE> : public BaseParameter {
    public:
    Param(jsonobj_t _json) ;
    Param(str_t jsonfile);

    void ParseFromJson(jsonobj_t _json) override;
    virtual value_t EngValue(const DArincData& Data) override;
    virtual void setEngValue(const value_t& value, DArincData& data)  override;
    inline const std::map<value_t, str_t>& GetInvDetails(){
        return InvDetails;
    }
    str_t getInfo() override;

protected:
    std::map<value_t, str_t> InvDetails;
};
