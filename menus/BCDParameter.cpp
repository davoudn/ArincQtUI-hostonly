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
    ParseFromJson(json);
}

void param_t::ParseFromJson(jsonobj_t json){
     if (!Assert<param_t>(json)){
        std::cout << "BCDParameter::ParseFromJson : assertion failed. \n";
        exit(1);
     }
     
     Type = DArincParamType::BCD;
     Name = json["Name"].toString();

     auto detailsobj = json["Details"].toObject();
     for (auto& pname: BNRBCD_PARAMETER_NAMES){
       Details[pname] = detailsobj[pname].toVariant();
     }

     SigBits =  getDetail("SigBits").toInt();
     IBit = BCDBNR_MSB - SigBits * 4 + 1;
     FBit = BCDBNR_MSB;
}



value_t param_t::EngValue(const DArincData& Data){
    if (Data.GetBit(30) == 0 && Data.GetBit(31) == 0){
        return  ArincFunctions::BCD::Value<value_t>(SigBits, Data).toDouble() * getDetail("Resolution").toDouble();  //ArincFunctions::BCD::getMax(SigBits) * getDetail("Scale+").toDouble();
    }

    if (Data.GetBit(30) == 1 && Data.GetBit(31) == 1){
        return -ArincFunctions::BCD::Value<value_t>(SigBits, Data).toDouble()* getDetail("Resolution").toDouble(); // ArincFunctions::BCD::getMax(SigBits) * getDetail("Scale-").toDouble();
    }

    return value_t(0.0);
}

void param_t::setEngValue(const value_t& value, DArincData& data){
    auto _value = value.toDouble();

    if ( _value >= 0.0 && (_value <= getDetail("Scale+").toDouble()) ){
        int __value = _value / getDetail("Resolution").toDouble();
        ArincFunctions::BCD::SetValue(value_t(__value), SigBits, data) ;
        return;
    }

    if ( (_value >= getDetail("Scale-").toDouble()) && _value < 0.0 ){
        int __value = _value / getDetail("Resolution").toDouble();
        ArincFunctions::BCD::SetValue(value_t(__value), SigBits, data) ;
        return;
    }

    if ( _value > getDetail("Scale+").toDouble()){
        int __value = getDetail("Scale+").toDouble() / getDetail("Resolution").toDouble();
        ArincFunctions::BCD::SetValue(value_t(__value), SigBits, data) ;
        return;
    }

    if ( _value < getDetail("Scale-").toDouble() ){
        int __value = getDetail("Scale-").toDouble() / getDetail("Resolution+").toDouble();
        ArincFunctions::BCD::SetValue(value_t(__value), SigBits, data) ;
            return;
    }

}

str_t param_t::getInfo(){
    return  "Units: " + getDetail("Units").toString()+ ", Resolution: " + this->getDetail("Resolution").toString() + " ,Scale-: " + getDetail("Scale-").toString() + ",Scale+: " + getDetail("Scale+").toString();
}
