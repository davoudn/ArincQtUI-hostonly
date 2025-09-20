/* @brief  
*
*/
#include "BaseParameter.h"
#include "ArincData.h"
#include "arinc_functions.h"
#include <iostream>

/*
*/
std::ostream& operator << (std::ostream& os, const jsonobj_t& json){
  for (str_t& x : json.keys()){
      os << x.toStdString() << " : " << json[x].toString().toStdString() << ", ";
  }
  os << "\n";
  return os;
}

template <typename T>
bool Find(value_t key, T& t ){
    for (const auto& x: t){
        if (x == key){
            return true;
        }
    }
    return false;
}


/*
 * ItemType
*/

BaseParameter::BaseParameter():BaseItem(BaseItem::ItemType::Parameter){

}

value_t BaseParameter::BitsValue(const DArincData& data){
    return data.Get<Bits>(IBit, FBit);
}
void  BaseParameter::setBitsValue(const value_t& value, DArincData& data) {
    data.Set<Bits>(value, IBit, FBit);
}


value_t BaseParameter::getDetail(str_t _detailName){
    auto x0 = GetDetails().find(_detailName);
    if(x0!=GetDetails().end()){
        return x0->second;
    }
    return value_t();
}




