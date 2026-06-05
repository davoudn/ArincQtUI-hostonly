#include "INTParameter.h"
#include "arinc_functions.h"

using param_t = Param<DArincParamType::INT>;

template <>
bool Assert<param_t>(jsonobj_t json)
{
    if (!json.contains("Name")){
        return false;
    }
    if (!json.contains("Type"))
    {
        return false;
    }
    else
    {
        if ( json["Type"].toString() != "INT"){
            qDebug() << "Type: " << json["Type"].toString() << "is not INT.";
            return false;
        }
    }

    for (const auto& k: BNRBCD_PARAMETER_NAMES)
    {
        if (!json["Details"].toObject().contains(k)){
            qDebug() << "Key  " << k << "  Not found in INT parameter.\n";
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
    type = DArincParamType::INT;
}

void param_t::parseFromJson(jsonobj_t json)
{
    if (!Assert<param_t>(json)){
        qCritical() << "INTParameter::parseFromJson: assertion failed.";
        return;
    }
    
    type = DArincParamType::INT;
    name = json["Name"].toString();
    
    auto details_obj = json["Details"].toObject();
    for (auto& pname: BNRBCD_PARAMETER_NAMES){
        details[pname] = details_obj[pname].toVariant();
    }
    
    i_bit = getDetail("IBit").toInt();
    f_bit = getDetail("FBit").toInt();
}



value_t param_t::engValue(const DArincData& data)
{
    return ArincFunctions::INT::Value<value_t>(i_bit, f_bit, data);
}

void param_t::SetEngValue(const value_t& value, DArincData& data)
{
    auto _value = value.toDouble();

    if ( (_value >= GetDetail("Scale-").toDouble() ) && (_value <= GetDetail("Scale+").toDouble()) )
    {
        _value = (_value / GetDetail("Scale+").toDouble()) * ArincFunctions::BCD::getMax(GetDetail("SigBits").toInt());
        ArincFunctions::INT::SetValue(value_t(_value),i_bit,f_bit,data) ;
    }
    else
    {
        if ( _value > GetDetail("Scale+").toDouble()){
            _value = GetDetail("Scale+").toInt();
            ArincFunctions::INT::SetValue(value_t(_value),i_bit, f_bit, data) ;
        }
        if ( _value < GetDetail("Scale-").toDouble() ){
            _value = 0;
            ArincFunctions::INT::SetValue(value_t(_value), i_bit, f_bit, data) ;
        }
    }
}

str_t param_t::GetInfo()
{
    return  "Units: " + GetDetail("Units").toString()+ ", Resolution: " + this->GetDetail("Resolution").toString() + " ,Scale-: " + GetDetail("Scale-").toString() + ",Scale+: " + GetDetail("Scale+").toString();
}