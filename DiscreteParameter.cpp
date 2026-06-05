#include "DiscreteParameter.h"
#include "arinc_functions.h"
#include "utils.h"
#include "types.h"

using param_t = Param<DArincParamType::DISCRETE>;


template <>
bool Assert<param_t>(jsonobj_t json)
{
    for (const auto& k: DICRETE_PARAMETER_NAMES)
    {
        if (!json.contains(k)){
             qDebug()<< "Item" << k << "does not exist in : " << json;
             return false;
        }
    }
    return true;
}

param_t::Param(str_t json_file)
{
    str_t json_data = utils::ReadFile(json_file);
    jsondoc_t jdoc = jsondoc_t::fromJson( json_data.toUtf8());
    jsonobj_t jobj = jdoc.object();
    parseFromJson(jobj);
}

param_t::Param(jsonobj_t json)
{
    parseFromJson(json);
}


void param_t::parseFromJson(jsonobj_t json)
{
    if (!Assert<param_t>(json)){
        qCritical() << "DiscreteParameter::parseFromJson: assertion failed.";
        return;
    }
    
    type = DArincParamType::DISCRETE;
    name = json["Name"].toString();
    i_bit = json["IBit"].toInt();
    f_bit = json["FBit"].toInt();

    jsonobj_t obj_details = json["Details"].toObject();
    for (auto& x : obj_details.keys()){
        details[x] = obj_details[x].toVariant();
    }
}


value_t param_t::engValue(const DArincData& data)
{
    value_t val = ArincFunctions::BITS::Value<value_t>(i_bit, f_bit, data);
    auto x = details.find(val.toString());
    if ( x != details.end()) {
        return x->second;
    }
    else {
       return value_t("null");
    }
}

void param_t::SetEngValue(const value_t& value, DArincData& data)
{
    for (auto& x:GetDetails() ){
      if (x.second.toString() == value.toString()){
         ArincFunctions::BITS::SetValue<value_t>(value_t(x.first),i_bit,f_bit,data);
         qDebug ()<< "param_t::SetEngValue : "<< x.first<< "  " << value.toString() << "\n";
       }
    }
}

str_t param_t::GetInfo()
{
    return str_t();
}