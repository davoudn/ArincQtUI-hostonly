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
            qDebug() << "Assert<param_t> : Type, " << json["Type"].toString() << "is not BNR.";
            return false;
        }
    }

    for (const auto& k: BNRBCD_PARAMETER_NAMES)
    {
        if (!json["Details"].toObject().contains(k)){
            qDebug() << "Key  " << k << "  Not found in BCD parameter.\n";
            return false;
        }
    }
    return true;
}

param_t::Param(jsonobj_t json)
{
    ParseFromJson(json);
}



void param_t::ParseFromJson(jsonobj_t json)
{
    if (!Assert<param_t>(json)){
        std::cout << "BCDParameter::ParseFromJson : assertion failed. \n";
        exit(1);
    }

    Type = DArincParamType::FLOATBCD;
    Name = json["Name"].toString();

    auto detailsobj = json["Details"].toObject();
    for (auto& pname: BNRBCD_PARAMETER_NAMES){
        Details[pname] = detailsobj[pname].toVariant();
    }

    SigBits =  getDetail("SigBits").toInt();
    IBit = BCDBNR_MSB - SigBits * 4 + 1;
    FBit = BCDBNR_MSB;
}



value_t param_t::EngValue(const DArincData& Data)
{
    return (ArincFunctions::FLOATBCD::Value<value_t>(SigBits, Data).toDouble());
}

void param_t::setEngValue(const value_t& value, DArincData& data)
{
    auto _value = value.toDouble();

    if ( (_value >= getDetail("Scale-").toDouble() ) && (_value <= getDetail("Scale+").toDouble()) ){
        _value = (_value / getDetail("Scale+").toDouble()) * ArincFunctions::BCD::getMax(SigBits);
        ArincFunctions::FLOATBCD::SetValue(value_t(_value), SigBits, data) ;
    }
    else {
        if ( _value > getDetail("Scale+").toDouble()){
            _value = ArincFunctions::FLOATBCD::getMax(SigBits);
            ArincFunctions::FLOATBCD::SetValue(value_t(_value), SigBits, data) ;
        }
        if (_value < getDetail("Scale-").toDouble()){
            _value = ArincFunctions::FLOATBCD::getMax(SigBits);
            ArincFunctions::FLOATBCD::SetValue(value_t(_value), SigBits, data) ;
        }
    }

}

str_t param_t::getInfo()
{
    return  "Units: " + getDetail("Units").toString()+ ", Resolution: " + this->getDetail("Resolution").toString() + " ,Scale-: " + getDetail("Scale-").toString() + ",Scale+: " + getDetail("Scale+").toString();
}
