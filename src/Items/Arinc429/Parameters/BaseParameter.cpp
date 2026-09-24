#include "BaseParameter.h"
#include "ArincData.h"
#include "arinc_functions.h"
#include <iostream>

std::ostream& operator << (std::ostream& os, const jsonobj_t& json)
{
  for (str_t& x : json.keys())
  {
      os << x.toStdString() << " : " << json[x].toString().toStdString() << ", ";
  }
  os << "\n";
  return os;
}

template <typename T>
bool Find(value_t key, T& t )
{
    for (const auto& x: t)
    {
        if (x == key)
        {
            return true;
        }
    }
    return false;
}

BaseParameter::BaseParameter():
        BaseItem(BaseItem::ItemType::Parameter)
{
}

BaseParameter::BaseParameter(const str_t& name,
     int i_bit, int f_bit, const str_t& details)
    :BaseItem(BaseItem::ItemType::Parameter), 
    name(name), i_bit(i_bit), f_bit(f_bit)
{
    this->details["IBit"] = i_bit;
    this->details["FBit"] = f_bit;
    this->details["Details"] = details;
}

str_t BaseParameter::getName()
{
   return name;
}

const uint32_t& BaseParameter::getType() const
{
    return type;
}

value_t BaseParameter::getBitsValue(const DArincData& data) const
{
    return data.get<Bits>(i_bit, f_bit);
}

const std::map<str_t, value_t>& BaseParameter::getDetails() const
{
    return details;
}

value_t BaseParameter::getDetail(str_t detail_name) const
{
    auto x0 = getDetails().find(detail_name);
    if(x0!=getDetails().end())
    {
        return x0->second;
    }
    return value_t();
}

void BaseParameter::setBitsValue(const value_t& value, 
    DArincData& data) 
{
    data.set<Bits>(value, i_bit, f_bit);
}

