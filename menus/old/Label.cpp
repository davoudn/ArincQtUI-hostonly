#include "Label.h"
#include "ArincData.h"
#include "BNRParameter.h"
#include "BCDParameter.h"
#include "DiscreteParameter.h"
#include "FLOATBCDParameter.h"
#include "FLNOParameter.h"
#include "INTParameter.h"
#include "ATCBCDParameter.h"
#include "Equipment.h"
#include "BaseParameter.h"
#include "utils.h"

//#include "DEI1016.h"
#define DEBUG

template <typename T>
bool Find(str_t key, T& t ){
    
    for (const auto& x: t){
        if (x == key){
            return true;
        }
    }
    return false;
}

template <>
bool Assert<Label>(jsonobj_t json){
    
      
      if (!json.contains("Type")){
        return false;
      }
      if (!json.contains("Label")){
        return false;
      }
      if (!json.contains("Id")){
        return false;
      }

      return true;
}

/*
 *
 */

Label::~Label() {

}

Label::Label(str_t _path, str_t equipmentId, str_t labelId):BaseItem(BaseItem::ItemType::Label){
    //
    init(_path,equipmentId, labelId);
 }

int Label::parseFromJson(const str_t& filename) {

}

int  Label::init (str_t _path, str_t _equipmentId, str_t _labelId){
    path = _path;
    str_t filename = path + _labelId + "_" + _equipmentId + ".json";

    auto jobj = utils::GetJsonObj(filename);
    if (!Assert<Label>(jobj)){
        qDebug() << "Label::ParseFromJson: Assert Failed. Data: "<< filename <<"\n";
        exit(1);
    }

    EquipmentId  = jobj["Id"].toString();
    LabelId = jobj["Label"].toString();
    Type = jobj["Type"].toString();
    Name = jobj["Name"].toString();
    ArincData.Set<LabelIdOctal>(value_t(LabelId));

    addParams(jobj);

    for (auto& it : Params){
        ParamsNames.push_back(static_cast<BaseParameter*>(it)->GetName());
    }

    return 0;
}

void Label::addParams(jsonobj_t _jsonObj){
    // bnr, bcd
    BaseParameter* param = nullptr;
    auto params = _jsonObj["Params"].toObject();
    for(auto& x : params.keys()) {
        auto paramObj = params[x].toObject();
        if (paramObj["Type"].toString() == "BCD") {
            auto param = new Param<DArincParamType::BCD>(paramObj);
            param->parent = this;
            Params.push_back(param);
        }
        if (paramObj["Type"].toString() == "BNR") {
            auto param = new Param<DArincParamType::BNR>(paramObj);
            param->parent = this;
            Params.push_back(param);
        }
        if (paramObj["Type"].toString() == "DISCRETE") {
            auto param = new Param<DArincParamType::DISCRETE>(paramObj);
            param->parent = this;
            Params.push_back(param);
        }
        if (paramObj["Type"].toString() == "FLOATBCD") {
            auto param = new Param<DArincParamType::FLOATBCD>(paramObj);
            param->parent = this;
            Params.push_back(param);
        }
        if (paramObj["Type"].toString() == "FLNOBCD") {
            auto param = new Param<DArincParamType::FLNOBCD>(paramObj);
            param->parent = this;
            Params.push_back(param);
        }

        if (paramObj["Type"].toString() == "INT") {
            auto param = new Param<DArincParamType::INT>(paramObj);
            param->parent = this;
            Params.push_back(param);
        }

        if (paramObj["Type"].toString() == "ATCBCD") {
            auto param = new Param<DArincParamType::ATCBCD>(paramObj);
            param->parent = this;
            Params.push_back(param);
        }
    }
}


void Label::clear(){
    Params.clear();
}


const std::vector<str_t>& Label::getParametersNames() {
    return ParamsNames;
}

const PointerVector<BaseItem>& Label::getParameters() {
    return Params;
}

BaseItem* Label::getParameter(str_t pname) {
    for (std::vector<BaseItem*>::iterator it =  Params.begin(); it != Params.end(); it++){
        if ( static_cast<BaseParameter*>(*it)->GetName() == pname){
            return static_cast<BaseParameter*>(*it);
        }
    }
    return nullptr;
}

/*
 * value<> template specialization
*/
value_t Label::valueBits(str_t pname, DArincData& data) {
    for (PointerVector<BaseItem>::iterator it =  Params.begin(); it != Params.end(); it++){
        if ( static_cast<BaseParameter*>(*it)->GetName() == pname){
           return static_cast<BaseParameter*>(*it)->BitsValue (data);
        }
    }

    return "null";
}

value_t  Label::valueEng(str_t pname, DArincData& data) {
    for (PointerVector<BaseItem>::iterator it =  Params.begin(); it != Params.end(); it++){
        if ( static_cast<BaseParameter*>(*it)->GetName() == pname){
            return static_cast<BaseParameter*>(*it)->EngValue(data);
        }
    }

    return "null";
}

value_t Label::valueBits(str_t pname) {
    return valueBits(pname, ArincData);
}

value_t  Label::valueEng(str_t pname) {
    return valueEng(pname, ArincData);

}

void Label::setValueEng(str_t _pname, value_t _value){
    for (PointerVector<BaseItem>::iterator it =  Params.begin(); it != Params.end(); it++){
        if ( static_cast<BaseParameter*>(*it)->GetName() == _pname){
            static_cast<BaseParameter*>(*it)->setEngValue(_value, ArincData);
        }
    }
}


value_t Label::valueBits(){
     str_t tmp;
     for (uint32_t i=SDI_LSB; i<=ArincData.Size();i++){
         tmp+= str_t::number(ArincData.GetBit(i));
     }
     return tmp;
}

void Label::setValueBits(value_t _bits){
    str_t _bitsString = _bits.toString();

        for (uint32_t i=SDI_LSB; i<=ARINC32_SIZE;i++){
             if (_bitsString[i-SDI_LSB] == '0')
                ArincData.SetBit(i,0);
             if (_bitsString[i-SDI_LSB] == '1')
                ArincData.SetBit(i,1);
        }
}

void Label::setArincData(DArincData arincdata){
    ArincData = arincdata;
    activate(true);

}

str_t Label::getStatus(){
    if (bIfActive){
        return "Active";
    }
    else {
        return "Idle";
    }
}

str_t Label::getName(){
    return LabelId + "( " + Name + " )";
}



