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
#include "Database.h"

#define DEBUG

template <typename T>
bool Find(str_t key, T& t )
{
    
    for (const auto& x: t){
        if (x == key){
            return true;
        }
    }
    return false;
}

template <>
bool Assert<Label>(jsonobj_t json)
{
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

Label::~Label()
{

}

void Label::timerCallback()
{
    
}

Label::Label(str_t path, str_t equipment_id, str_t label_id):BaseItem(BaseItem::ItemType::Label)
{
    init(path,equipment_id, label_id);
}

int Label::parseFromJson(const str_t& json_data)
{
    return 0;
}

int Label::init(str_t path, str_t equipment_id, str_t label_id)
{
    return initFromDatabase(path, equipment_id, label_id);
}

int Label::initFromDatabase(str_t path, str_t equipment_id, str_t label_id)
{
    clear();
    this->path = path;
    
    auto* db = Database::getInstance();
    if (!db->openDatabase(path)) {
        qCritical() << "Failed to open database at: " << path;
        return -1;
    }
    
    auto label_data = db->getLabel(label_id, equipment_id);
    if (!label_data) {
        qCritical() << "Label not found in database: " << label_id << " for equipment: " << equipment_id;
        return -1;
    }
    
    this->equipment_id  = label_data->equipment_id;
    if (equipment_id == "1111"){
        this->label_id = label_id;
    }
    else {
        this->label_id = label_data->id;
    }
    this->type = label_data->type;
    this->name = label_data->name;
    arinc_data.set<LabelIdOctal>(value_t(label_id));
    
    auto params_list = db->getParameters(label_id, equipment_id);
    for (const auto& param : params_list) {
        BaseParameter* base_param = nullptr;
        
        if (param.type == "BCD") {
            base_param = new Param<DArincParamType::BCD>(param.name, param.i_bit, param.f_bit, param.details);
        } else if (param.type == "BNR") {
            base_param = new Param<DArincParamType::BNR>(param.name, param.i_bit, param.f_bit, param.details);
        } else if (param.type == "DISCRETE") {
            jsonobj_t dj;
            dj["Name"] = param.name;
            dj["Type"] = "DISCRETE";
            dj["Details"] = param.details;
            base_param = new Param<DArincParamType::DISCRETE>(dj);
        } else if (param.type == "INT") {
            base_param = new Param<DArincParamType::INT>(param.name, param.i_bit, param.f_bit, param.details);
        }
        
        if (base_param) {
            base_param->parent = this;
            params.push_back(base_param);
            params_names.push_back(base_param->getName());
        }
    }
    return 0;
}

void Label::addParams(jsonobj_t json_obj)
{
    auto params_obj = json_obj["Params"].toObject();
    for(auto& x : params_obj.keys())
    {
        auto param_obj = params_obj[x].toObject();
        BaseParameter* base_param = nullptr;
        if (param_obj["Type"].toString() == "BCD") {
            base_param = new Param<DArincParamType::BCD>(param_obj);
        }
        else if (param_obj["Type"].toString() == "BNR") {
            base_param = new Param<DArincParamType::BNR>(param_obj);
        }
        else if (param_obj["Type"].toString() == "DISCRETE") {
            base_param = new Param<DArincParamType::DISCRETE>(param_obj);
        }
        else if (param_obj["Type"].toString() == "FLOATBCD") {
            base_param = new Param<DArincParamType::FLOATBCD>(param_obj);
        }
        else if (param_obj["Type"].toString() == "FLNOBCD") {
            base_param = new Param<DArincParamType::FLNOBCD>(param_obj);
        }
        else if (param_obj["Type"].toString() == "INT") {
            base_param = new Param<DArincParamType::INT>(param_obj);
        }
        else if (param_obj["Type"].toString() == "ATCBCD") {
            base_param = new Param<DArincParamType::ATCBCD>(param_obj);
        }
        
        if (base_param) {
            base_param->parent = this;
            params.push_back(base_param);
            params_names.push_back(base_param->getName());
        }
    }
}

void Label::clear()
{
    params.clear();
}

const std::vector<str_t>& Label::getParametersNames() const
{
    return params_names;
}

const PointerVector<BaseItem>& Label::getParameters() const
{
    return params;
}

BaseItem* Label::getParameter(str_t pname)
{
    for (auto it = params.begin(); it != params.end(); it++)
    {
        if ( static_cast<BaseParameter*>(*it)->getName() == pname){
            return static_cast<BaseParameter*>(*it);
        }
    }
    return nullptr;
}

value_t Label::getValueBits(str_t pname) const
{
    for (auto it = params.begin(); it != params.end(); it++){
        if ( static_cast<BaseParameter*>(*it)->getName() == pname){
           return static_cast<BaseParameter*>(*it)->getBitsValue(arinc_data);
        }
    }
    return "null";
}

value_t Label::getValueEng(str_t pname) const
{
    for (auto it = params.begin(); it != params.end(); it++)
    {
        if ( static_cast<BaseParameter*>(*it)->getName() == pname){
            return static_cast<BaseParameter*>(*it)->getEngValue(arinc_data);
        }
    }
    return "null";
}

value_t Label::getValueBits() const
{
    str_t tmp;
    for (uint32_t i=SDI_LSB; i<=arinc_data.size();i++)
    {
        tmp+= str_t::number(arinc_data.getBit(i));
    }
    return tmp;
}

void Label::setValueEng(str_t pname, value_t value)
{
    for (auto it = params.begin(); it != params.end(); it++)
    {
        if ( static_cast<BaseParameter*>(*it)->getName() == pname)
        {
            static_cast<BaseParameter*>(*it)->setEngValue(value, arinc_data);
        }
    }
}


void Label::setValueBits(value_t bits)
{
    str_t bits_string = bits.toString();
    for (uint32_t i=SDI_LSB; i<=ARINC32_SIZE;i++)
    {
        if (bits_string[i-SDI_LSB] == '0')
                arinc_data.setBit(i,0);
        if (bits_string[i-SDI_LSB] == '1')
                arinc_data.setBit(i,1);
    }
}

void Label::setArincData(DArincData data)
{
    arinc_data = data;
    activate(true);
}

str_t Label::getStatus() const 
{
    if (if_active){
        return "Active";
    }
    else {
        return "Idle";
    }
}

str_t Label::getName() const
{
    return label_id + "( " + name + " )";
}

DArincData& Label::getArincData()
{
        return arinc_data;
}

uint32_t Label::getUIntArincData() const
{
    return static_cast<uint32_t>(arinc_data.template get<int>().toUInt());
}
bool Label::getIfActive() const
{
    return if_active;
}
value_t Label::getDataRate() const 
{
    return value_t(data_rate);
}
void Label::setStatus(str_t status)
{
    if (status == "Active"){
        if_active = true;
    }
    if (status == "Idle"){
        if_active = false;
    }
}
void Label::activate(bool active)
{
   if_active = active;
}
void Label::setDataRate(value_t value)
{
    data_rate = value.toDouble();
}
void Label::resetDataRateCounter(){
    data_rate_counter = 0;
}
void Label::incrementDataRateCounter()
{
    data_rate_counter++;
}
