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

param_t::Param(jsonobj_t json) {
    parseFromJson(json);
}

param_t::Param(const str_t& name, int i_bit, int f_bit, 
    const str_t& details)
    :BaseParameter(name, i_bit, f_bit, details)
{
    type = DArincParamType::BNR;
}

void param_t::parseFromJson(jsonobj_t json) {
    if (!Assert<param_t>(json)){
        qCritical() << "BNRParameter::parseFromJson: assertion failed.";
        return;
    }
    type = DArincParamType::BNR;
    name = json["Name"].toString();

    auto details_obj = json["Details"].toObject();
    for (auto& pname: BNRBCD_PARAMETER_NAMES){
        details[pname] = details_obj[pname].toVariant();
    }

    uint8_t y = getDetail("SigBits").toUInt();
    i_bit = BCDBNR_MSB - y;
    f_bit = BCDBNR_MSB ;
}

void param_t::setEngValue(const value_t& value, DArincData& data)
{
    auto _value = value.toDouble();
    if (_value>0.f && getDetail("Scale+").toDouble()>0.0){
        _value = _value / getDetail("Scale+").toDouble();
        ArincFunctions::BNR::SetValue(value_t(_value),i_bit,f_bit,data) ;
    }
    if (_value<0.f && getDetail("Scale-").toDouble()<0){
        _value = -_value / getDetail("Scale-").toDouble();
        ArincFunctions::BNR::SetValue(value_t(_value),i_bit,f_bit,data) ;
    }
    if (_value == 0.f){
        ArincFunctions::BNR::SetValue(value_t(_value),i_bit,f_bit,data) ;
    }
}

value_t param_t::getEngValue(const DArincData& data) const 
{
    double result = 0.0;
    if(data.getBit(f_bit) == 0){
        result = ArincFunctions::BNR::Value<double>(i_bit, f_bit, data) * getDetail("Scale+").toDouble();
    }
    else {
        result = ArincFunctions::BNR::Value<double>(i_bit, f_bit, data) * getDetail("Scale-").toDouble();
    }
    return value_t(result);
}

str_t param_t::getInfo()
{
    return  "Units: " + getDetail("Units").toString() 
            + ", Resolution: " + this->getDetail("Resolution").toString()
            + ",Scale-: " + getDetail("Scale-").toString() 
            + ",Scale+: "  + getDetail("Scale+").toString();
}