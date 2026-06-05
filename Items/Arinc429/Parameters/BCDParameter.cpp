#include "BCDParameter.h"
#include "arinc_functions.h"
#include "types.h"

template <typename T>
bool Find(str_t key, T& t ){
    for (const auto& x: t){
        if (x == key){
            return true;
        }
    }
    return false;
}

using param_t = Param<DArincParamType::BCD>;

template <>
bool Assert<param_t>(jsonobj_t json){
    if (!json.contains("Name")){
      return false;
    }
    if (!json.contains("Type")){
      return false;
    }
    else{
      if ( json["Type"].toString() != "BCD"){
          qDebug() << "Type: " << json["Type"].toString() << "is not BCD.";
          return false;
      }
    }
    
    for (const auto& k: BNRBCD_PARAMETER_NAMES){
        if (!json["Details"].toObject().contains(k)){
            qDebug() << "Key  " << k << "  Not found in BCD parameter.\n";
            return false;
        }
    }
    return true;
}

param_t::Param(jsonobj_t json){
    parseFromJson(json);
}

param_t::Param(const str_t& name, int i_bit, int f_bit, const str_t& details)
    : BaseParameter(name, i_bit, f_bit, details)
{
    type = DArincParamType::BCD;
    sig_bits = GetDetail("SigBits").toInt();
}

void param_t::parseFromJson(jsonobj_t json){
     if (!Assert<param_t>(json)){
         qCritical() << "BCDParameter::parseFromJson: assertion failed.";
         return;
     }
     
     type = DArincParamType::BCD;
     name = json["Name"].toString();
 
     auto details_obj = json["Details"].toObject();
     for (auto& pname: BNRBCD_PARAMETER_NAMES){
         details[pname] = details_obj[pname].toVariant();
     }
 
     sig_bits =  getDetail("SigBits").toInt();
     i_bit = BCDBNR_MSB - sig_bits * 4 + 1;
     f_bit = BCDBNR_MSB;
 }



value_t param_t::getEngValue(const DArincData& data){
    if (data.getBit(30) == 0 && data.getBit(31) == 0){
        return  ArincFunctions::BCD::Value<value_t>(sig_bits, data).toDouble() * getDetail("Resolution").toDouble();
    }

    if (data.getBit(30) == 1 && data.getBit(31) == 1){
        return -ArincFunctions::BCD::Value<value_t>(sig_bits, data).toDouble()* getDetail("Resolution").toDouble();
    }

    return value_t(0.0);
}

void param_t::setEngValue(const value_t& value, DArincData& data){
    auto _value = value.toDouble();

    if ( _value >= 0.0 && (_value <= getDetail("Scale+").toDouble()) ){
        int __value = _value / getDetail("Resolution").toDouble();
        ArincFunctions::BCD::SetValue(value_t(__value), sig_bits, data) ;
        return;
    }

    if ( (_value >= getDetail("Scale-").toDouble()) && _value < 0.0 ){
        int __value = _value / getDetail("Resolution").toDouble();
        ArincFunctions::BCD::SetValue(value_t(__value), sig_bits, data) ;
        return;
    }

    if ( _value > getDetail("Scale+").toDouble()){
        int __value = getDetail("Scale+").toDouble() / getDetail("Resolution").toDouble();
        ArincFunctions::BCD::SetValue(value_t(__value), sig_bits, data) ;
        return;
    }

    if ( _value < getDetail("Scale-").toDouble() ){
        int __value = getDetail("Scale-").toDouble() / getDetail("Resolution+").toDouble();
        ArincFunctions::BCD::SetValue(value_t(__value), sig_bits, data) ;
            return;
    }
}

    if (data.getBit(30) == 1 && data.getBit(31) == 1){
        return -ArincFunctions::BCD::Value<value_t>(sig_bits, data).toDouble()* getDetail("Resolution").toDouble();
    }

    return value_t(0.0);
}

void param_t::SetEngValue(const value_t& value, DArincData& data){
    auto _value = value.toDouble();

    if ( _value >= 0.0 && (_value <= GetDetail("Scale+").toDouble()) ){
        int __value = _value / GetDetail("Resolution").toDouble();
        ArincFunctions::BCD::SetValue(value_t(__value), sig_bits, data) ;
        return;
    }

    if ( (_value >= GetDetail("Scale-").toDouble()) && _value < 0.0 ){
        int __value = _value / GetDetail("Resolution").toDouble();
        ArincFunctions::BCD::SetValue(value_t(__value), sig_bits, data) ;
        return;
    }

    if ( _value > GetDetail("Scale+").toDouble()){
        int __value = GetDetail("Scale+").toDouble() / GetDetail("Resolution").toDouble();
        ArincFunctions::BCD::SetValue(value_t(__value), sig_bits, data) ;
        return;
    }

    if ( _value < GetDetail("Scale-").toDouble() ){
        int __value = GetDetail("Scale-").toDouble() / GetDetail("Resolution+").toDouble();
        ArincFunctions::BCD::SetValue(value_t(__value), sig_bits, data) ;
            return;
    }

}

str_t param_t::GetInfo(){
    return  "Units: " + GetDetail("Units").toString()+ ", Resolution: " + this->GetDetail("Resolution").toString() + " ,Scale-: " + GetDetail("Scale-").toString() + ",Scale+: " + GetDetail("Scale+").toString();
}