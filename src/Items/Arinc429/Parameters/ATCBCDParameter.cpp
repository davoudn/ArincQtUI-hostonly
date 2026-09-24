#include "ATCBCDParameter.h"

#include "arinc_functions.h"


using param_t = Param<DArincParamType::ATCBCD>;

template <>
bool Assert<param_t>(jsonobj_t json){

    if (!json.contains("Name")){
        return false;
    }
    if (!json.contains("Type")){
        return false;
    }
    else{
        if ( json["Type"].toString() != "ATCBCD"){
            qDebug() << "Assert<param_t> : Type, " << json["Type"].toString() << "is not ATCBCD.";
            return false;
        }
    }

    for (const auto& k: BNRBCD_PARAMETER_NAMES){
        if (!json["Details"].toObject().contains(k)){
            qDebug() << "Key  " << k << "  Not found in ATCBCD parameter.\n";
            return false;
        }
    }
    return true;
}

param_t::Param(jsonobj_t json) {
    parseFromJson(json);
}

param_t::Param(const str_t& name, int i_bit, int f_bit, const str_t& details)
    : BaseParameter(name, i_bit, f_bit, details)
{
    type = DArincParamType::ATCBCD;
}



void param_t::parseFromJson(jsonobj_t json){
    if (!Assert<param_t>(json)){
        qCritical() << "ATCBCDParameter::parseFromJson: assertion failed.";
        return;
    }

    type = DArincParamType::ATCBCD;
    name = json["Name"].toString();

    auto details_obj = json["Details"].toObject();
    for (auto& pname: BNRBCD_PARAMETER_NAMES){
        details[pname] = details_obj[pname].toVariant();
    }

    sig_bits = getDetail("SigBits").toInt();
    i_bit = 0;
    f_bit = 0;
}



value_t param_t::getEngValue(const DArincData& data) const {
    return (ArincFunctions::ATCBCD::Value<value_t>(data).toDouble());
}

void param_t::setEngValue(const value_t& value, DArincData& data) {
    auto _value = value.toDouble();

    if ( (_value >= getDetail("Scale-").toDouble() ) && (_value <= getDetail("Scale+").toDouble()) ){
        ArincFunctions::ATCBCD::SetValue(value_t(_value), data) ;
    }
    else {
        if ( _value > getDetail("Scale+").toDouble()){
            _value = 7777;
            ArincFunctions::ATCBCD::SetValue(value_t(_value), data) ;
        }
        if (_value < getDetail("Scale-").toDouble()){
            _value = 0;
            ArincFunctions::ATCBCD::SetValue(value_t(_value), data) ;
        }
    }
}

str_t param_t::getInfo(){
    return  "Units: " + getDetail("Units").toString()+ ", Resolution: " + this->getDetail("Resolution").toString() + " ,Scale-: " + getDetail("Scale-").toString() + ",Scale+: " + getDetail("Scale+").toString();
}
