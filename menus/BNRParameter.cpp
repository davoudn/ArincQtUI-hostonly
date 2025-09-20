#include "BNRParameter.h"
#include "arinc_functions.h"
#include "types.h"

using param_t = Param<DArincParamType::BNR>;


template <>
bool Assert<param_t>(jsonobj_t json){
  
      if (!json.contains("Name")){
        return false;
      }
      if (!json.contains("Type")){
        return false;
      }
      else{
        if ( json["Type"].toString() != "BNR"){
            qDebug() << "Assert<param_t> : Type, " << json["Type"].toString() << "is not BNR.";
            return false;
        }
      }
      
      for (const auto& k: BNRBCD_PARAMETER_NAMES){
         if (!json["Details"].toObject().contains(k)){
             return false;
         }
      }
      return true;
}
//
param_t::Param(jsonobj_t json) {
    ParseFromJson(json);
}
//
void param_t::ParseFromJson(jsonobj_t json) {
     if (!Assert<param_t>(json)){
        std::cout << "BaseParameter::Assert<DArincParamType::BNR> : assertion failed. \n";
        exit(1);
     }
     //
     Type = DArincParamType::BNR;
     Name = json["Name"].toString();

     auto detailsobj = json["Details"].toObject();
     for (auto& pname: BNRBCD_PARAMETER_NAMES){
         Details[pname] =detailsobj[pname].toVariant();
     }

     uint8_t y = getDetail("SigBits").toUInt();
     IBit = BCDBNR_MSB - y;
     FBit = BCDBNR_MSB ;
}
//
value_t param_t::EngValue(const DArincData& Data) {
 double result = 0.0;
 if(Data.GetBit(FBit) == 0){
     result = ArincFunctions::BNR::Value<double>(IBit, FBit, Data) *  getDetail("Scale+").toDouble();
 }
 else {
     result = ArincFunctions::BNR::Value<double>(IBit, FBit, Data) * getDetail("Scale-").toDouble();
 }
    return value_t(result);
}
//
void param_t::setEngValue(const value_t& value, DArincData& data){
    auto _value = value.toDouble();
    if (_value>0.f && getDetail("Scale+").toDouble()>0.0){
       _value = _value / getDetail("Scale+").toDouble();
       ArincFunctions::BNR::SetValue(value_t(_value),IBit,FBit,data) ;
    }
    //
    if (_value<0.f && getDetail("Scale-").toDouble()<0){
       _value = -_value / getDetail("Scale-").toDouble();
       ArincFunctions::BNR::SetValue(value_t(_value),IBit,FBit,data) ;
    }
    //
    if (_value == 0.f){
       ArincFunctions::BNR::SetValue(value_t(_value),IBit,FBit,data) ;
    }
    //
}
//
str_t param_t::getInfo(){
    return  "Units: " + getDetail("Units").toString()+ ", Resolution: " + this->getDetail("Resolution").toString() + " ,Scale-: " + getDetail("Scale-").toString() + ",Scale+: " + getDetail("Scale+").toString();
}

