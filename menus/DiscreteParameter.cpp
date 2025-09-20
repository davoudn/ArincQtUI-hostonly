#include "DiscreteParameter.h"
#include "arinc_functions.h"
#include "utils.h"
#include "types.h"

using param_t = Param<DArincParamType::DISCRETE>;


template <typename T>
bool Find(str_t key, T& t )
{
    for (const auto& x: t)
    {
        if (x == key){
            return true;
        }
    }
    return false;
}

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

param_t::Param(str_t jsonfile)
{
    str_t jsonData = utils::ReadFile(jsonfile);
    jsondoc_t jdoc = jsondoc_t::fromJson( jsonData.toUtf8());
    jsonobj_t jobj = jdoc.object();
    ParseFromJson(jobj);
}

param_t::Param(jsonobj_t json)
{
    ParseFromJson(json);
}


void param_t::ParseFromJson(jsonobj_t json)
{
    if (!Assert<param_t>(json)){
        std::cout << "BaseParameter::ParseFromJson<DArincParamType::DISCRETE> : assertion failed. \n";
    //    exit(1);
     }
    
    Type = DArincParamType::DISCRETE;
    Name = json["Name"].toString();
    IBit = json["IBit"].toInt();
    FBit = json["FBit"].toInt();

    jsonobj_t ObjDetails = json["Details"].toObject();
    for (auto& x : ObjDetails.keys()){
        Details[x] = ObjDetails[x].toVariant();
        //InvDetails[ObjDetails[x].toVariant()] = x;
    }
}


value_t param_t::EngValue(const DArincData& Data)
{
    value_t val = ArincFunctions::BITS::Value<value_t>(IBit, FBit, Data);
    auto x = Details.find(val.toString());
    if ( x != Details.end()) {
        return x->second;
    }
    else {
       return value_t("null");
    }
}

void param_t::setEngValue(const value_t& value, DArincData& data)
{
    for (auto& x:GetDetails() ){
      if (x.second.toString() == value.toString()){
         ArincFunctions::BITS::SetValue<value_t>(value_t(x.first),IBit,FBit,data);
         qDebug ()<< "param_t::setEngValue : "<< x.first<< "  " << value.toString() << "\n";
      }
    }
}

str_t param_t::getInfo()
{
    return str_t();
}

