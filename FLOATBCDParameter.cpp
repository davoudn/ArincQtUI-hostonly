#include "FLOATBCDParameter.h"

using param_t = Param<DArincParamType::FLOATBCD>;

template <>
bool Assert<param_t>(jsonobj_t json)
{

    if (!json.contains("Name")){
        return false;
    }
    //
    if (!json.contains("Type"))
    {
        return false;
    }
    else
    {
        if ( json["Type"].toString() != "FLOATBCD"){
            qDebug() << "Assert<param_t> : Type, " << json["Type"].toString() << "is not FLOATBCD.";
            return false;
        }
    }

    for (const auto& k: BNRBCD_PARAMETER_NAMES)
    {
        if (!json["Details"].toObject().contains(k)){
            qDebug() << "Key  " << k << "  Not found in FLOATBCD parameter.\n";
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
    type = DArincParamType::FLOATBCD;
    sig_bits = GetDetail("SigBits").toInt();
}



void param_t::parseFromJson(jsonobj_t json)
{
    if (!Assert<param_t>(json)){
        qCritical() << "FLOATBCDParameter::parseFromJson: assertion failed.";
        return;
    }
    
    type = DArincParamType::FLOATBCD;
    name = json["Name"].toString();
    
    auto details_obj = json["Details"].toObject();
    for (auto& pname: BNRBCD_PARAMETER_NAMES){
        details[pname] = details_obj[pname].toVariant();
    }
    
    sig_bits = getDetail("SigBits").toInt();
    i_bit = BCDBNR_MSB - sig_bits * 4 + 1;
    f_bit = BCDBNR_MSB;
}



value_t param_t::engValue(const DArincData& data)
{
    return (ArincFunctions::FLOATBCD::Value<value_t>(sig_bits, data).toDouble());
}

void param_t::SetEngValue(const value_t& value, DArincData& data)
{
    auto _value = value.toDouble();

    if ( (_value >= GetDetail("Scale-").toDouble() ) && (_value <= GetDetail("Scale+").toDouble()) ){
        _value = (_value / GetDetail("Scale+").toDouble()) * ArincFunctions::BCD::getMax(sig_bits);
        ArincFunctions::FLOATBCD::SetValue(value_t(_value), sig_bits, data) ;
    }
    else {
        if ( _value > GetDetail("Scale+").toDouble()){
            _value = ArincFunctions::FLOATBCD::getMax(sig_bits);
            ArincFunctions::FLOATBCD::SetValue(value_t(_value), sig_bits, data) ;
        }
        if (_value < GetDetail("Scale-").toDouble()){
            _value = ArincFunctions::FLOATBCD::getMax(sig_bits);
            ArincFunctions::FLOATBCD::SetValue(value_t(_value), sig_bits, data) ;
        }
    }

}

str_t param_t::GetInfo()
{
    return  "Units: " + GetDetail("Units").toString()+ ", Resolution: " + this->GetDetail("Resolution").toString() + " ,Scale-: " + GetDetail("Scale-").toString() + ",Scale+: " + GetDetail("Scale+").toString();
}