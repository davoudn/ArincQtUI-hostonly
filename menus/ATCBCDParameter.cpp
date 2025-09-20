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
            qDebug() << "Assert<param_t> : Type, " << json["Type"].toString() << "is not BNR.";
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

param_t::Param(jsonobj_t json) {
    ParseFromJson(json);
}



void param_t::ParseFromJson(jsonobj_t json){
    if (!Assert<param_t>(json)){
        std::cout << "BCDParameter::ParseFromJson : assertion failed. \n";
        exit(1);
    }

    Type = DArincParamType::ATCBCD;
    Name = json["Name"].toString();

    auto detailsobj = json["Details"].toObject();
    for (auto& pname: BNRBCD_PARAMETER_NAMES){
        Details[pname] = detailsobj[pname].toVariant();
    }

    SigBits =  getDetail("SigBits").toInt();
    IBit = 0;//BCDBNR_MSB - SigBits * 4 + 1;
    FBit = 0;//BCDBNR_MSB;
}



value_t param_t::EngValue(const DArincData& Data){
    return (ArincFunctions::ATCBCD ::Value<value_t>(Data).toDouble());
}

void param_t::setEngValue(const value_t& value, DArincData& data){
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
