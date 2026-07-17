#include "FLNOParameter.h"
#include "arinc_functions.h"



using param_t = Param<DArincParamType::FLNOBCD>;

template <>
bool Assert<param_t>(jsonobj_t json)
{
    if (!json.contains("Name")){
        return false;
    }
    if (!json.contains("Type")){
        return false;
    }
    else{
        if ( json["Type"].toString() != "FLNOBCD"){
            qDebug() << "Type: " << json["Type"].toString() << "is not FLNOBCD.";
            return false;
        }
    }

    for (const auto& k: BNRBCD_PARAMETER_NAMES){
        if (!json["Details"].toObject().contains(k)){
            qDebug() << "Key  " << k << "  Not found in FLNOBCD parameter.\n";
            return false;
        }
    }
    return true;
}

param_t::Param(jsonobj_t json)
{
    parseFromJson(json);
}

param_t::Param(const str_t& name, int i_bit, int f_bit, const str_t& details)
    : BaseParameter(name, i_bit, f_bit, details)
{
    type = DArincParamType::FLNOBCD;
    sig_bits = getDetail("SigBits").toInt();
}

void param_t::parseFromJson(jsonobj_t json)
{
    if (!Assert<param_t>(json)){
        qCritical() << "FLNOParameter::parseFromJson: assertion failed.";
        return;
    }
    
    type = DArincParamType::FLNOBCD;
    name = json["Name"].toString();
    
    auto details_obj = json["Details"].toObject();
    for (auto& pname: BNRBCD_PARAMETER_NAMES){
        details[pname] = details_obj[pname].toVariant();
    }
    
    sig_bits = getDetail("SigBits").toInt();
    i_bit = BCDBNR_MSB - sig_bits * 4 + 1;
    f_bit = BCDBNR_MSB;
}



value_t param_t::getEngValue(const DArincData& data) const
{
    return ArincFunctions::FLNOBCD::Value<value_t>(data).toDouble();
}

void param_t::setEngValue(const value_t& value, DArincData& data)
{
    auto _value = value.toDouble();

    if ( (_value >= getDetail("Scale-").toDouble() ) && (_value <= getDetail("Scale+").toDouble()) ){
        _value = (_value / getDetail("Scale+").toDouble()) * ArincFunctions::BCD::getMax(sig_bits);
        ArincFunctions::FLNOBCD::SetValue(value_t(_value), data) ;
    }
    else {
        if ( _value > getDetail("Scale+").toDouble()){
            _value = ArincFunctions::BCD::getMax(sig_bits);
            ArincFunctions::FLNOBCD::SetValue(value_t(_value), data) ;
        }
        if ( _value < getDetail("Scale-").toDouble() ){
            _value = 9999;
            ArincFunctions::FLNOBCD::SetValue(value_t(_value), data) ;
        }
    }
}

str_t param_t::getInfo()
{
    return  "Units: " + getDetail("Units").toString()+ ", Resolution: " + this->getDetail("Resolution").toString() + " ,Scale-: " + getDetail("Scale-").toString() + ",Scale+: " + getDetail("Scale+").toString();
}