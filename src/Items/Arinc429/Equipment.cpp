#include "Equipment.h"
#include "ArincData.h"
#include "arinc_functions.h"
#include "Receiver.h"
#include "utils.h"
#include "generaldata.h"
#include "BaseParameter.h"

#include "labelfor.h"

#include <QThread>


void Log(Label* label, DArincData& arinc_data)
{
    if (label)
    {
        qDebug()<< "Label Id: " << label->label_id << "Label Name: " << label->name << " Label Type:" << label->type << "/*\n";
        for (auto& p : label->getParametersNames()) {
            qDebug() << "Parameter Name: " << p << " Engineering Value: " << label->getValueEng(p) << "\n";
            qDebug() << "Parameter Name: " << p << " Raw Value        : " << label->getValueBits(p) << "\n";
        }
        qDebug() << "*/\n";
    }
    else
    {
        qDebug() << "Label Id: " << arinc_data.get<LabelIdOctal>() << "is not part of this equipment, the raw value is: /* \n";
        qDebug() << " Value: " << arinc_data.get<DataBits>() << "\n";
    }
}

str_t Equipment::readName(str_t equipment_id)
{
    jsonobj_t jobj = utils::GetJsonObj(path+ "equipment_names.json");
    
    if (jobj.contains(equipment_id) ){
        return jobj[equipment_id].toString();
    }
    
    return str_t("Unknown_Name");
}

Equipment::Equipment(str_t equipment_id, EquipmentRole role): BaseItem(BaseItem::ItemType::Equipment)
{
    path = GeneralData::getInstance()->LABELS_PATH;
    this->role = role;
    init(equipment_id);
}

str_t Equipment::loadName(str_t label_id, str_t equipment_id)
{
    str_t filename = path + label_id + "_" + equipment_id + ".json";
    
    auto jobj = utils::GetJsonObj(filename);
    if (!Assert<Label>(jobj)){
        qCritical() << "Equipment::loadName: Label assert Failed. Data: "<< filename;
        return str_t();
    }
    str_t tmp = label_id + "( " + jobj["Name"].toString() + " )";
    return tmp;
}

void Equipment::init (str_t equipment_id)
{
    clear();
    this->equipment_id    = equipment_id;
    equipment_name  = readName(equipment_id);
    auto jobj      = utils::GetJsonObj(path+"equipment.json");
    
    if (jobj.contains(equipment_id) ){
        int c = 0;
        for (const auto& label_id: jobj[equipment_id].toArray()){
            labels_list.push_back(label_id.toString());
            labels_id_and_names.emplace( loadName(label_id.toString(), equipment_id), label_id.toString());
            c++;
        }
    }
    else {
        qDebug() << "Equipment::init  : Equipment Id was not found. Only Raw values will be shown. \n";
    }
}

Label* Equipment::findLabel(str_t label_id)
{
    for (auto& x: labels ){
        if (static_cast<Label*>(x)->label_id == label_id){
            return static_cast<Label*>(x);
        }
    }
    return nullptr;
}

str_t Equipment::findLabelIdFromName(str_t id_and_name)
{
    auto x = labels_id_and_names.find(id_and_name);
    if (x!=labels_id_and_names.end()){
        return x->second;
    }
    return "null";
}

void Equipment::clear()
{
    labels.clear();
    labels_list.clear();
    labels_id_and_names.clear();
}

bool Equipment::addReservedLabel()
{
     Label* tmp = nullptr;
     tmp = new LabelFor<DReserved>();
    
    if (tmp){
            tmp->parent = this;
            labels.push_back(tmp);
            return true;
    }
     return false;
}

bool Equipment::addLabel(str_t label_id)
{
    Label* tmp = nullptr;
    if (checkLabel(label_id)){
        if (role == EquipmentRole::Receiver){
            tmp = new LabelFor<DReceiver>(path, equipment_id, label_id);
        }
        if (role == EquipmentRole::Transmitter){
            tmp = new LabelFor<DTransmitter>(path, equipment_id, label_id);
        }
        if (tmp){
            tmp->parent = this;
            labels.push_back(tmp);
            return true;
        }
    }
    return false;
}

bool Equipment::addLabel(str_t label_id, QThread* thread)
{
    Label* tmp = nullptr;
    if (checkLabel(label_id))
    {
        if (role == EquipmentRole::Receiver   ){
            tmp = new LabelFor<DReceiver>(path, equipment_id, label_id);
        }
        if (role == EquipmentRole::Transmitter){
            tmp = new LabelFor<DTransmitter>(path, equipment_id, label_id);
        }
        
        if (tmp){
            tmp->parent = this;
            labels.push_back(tmp);
            return true;
        }
    }
    return false;
}

void Equipment::insertLabel(uint32_t index, str_t label_id)
{
    Label* tmp = nullptr;
    
    if (checkLabel(label_id)){
        if (role == EquipmentRole::Receiver){
            tmp = new LabelFor<DReceiver>   (path, equipment_id, label_id);
        }
        if (role == EquipmentRole::Transmitter){
            tmp = new LabelFor<DTransmitter>(path, equipment_id, label_id);
        }
        
        if (tmp){
            tmp->parent = this;
            labels.insert(labels.cbegin(),tmp);
            
        }
    }
}

void Equipment::removeLabel(str_t label_id)
{
    Label* tmp = nullptr;
    for (auto& x: labels){
        if (static_cast<Label*>(x)->label_id == label_id){
            tmp = static_cast<Label*>(x);
        }
    }
    if (tmp){
        labels.remove(tmp);
    }
}

void Equipment::removeLabel(Label* label)
{
        labels.remove(label);
}

void Equipment::removeLabel(uint32_t row_id)
{
    qInfo() << labels.size() << "   " << row_id;
    if (row_id>=0 && row_id<labels.size()){
        if (labels[row_id]->type==BaseItem::ItemType::Label){
            labels.remove(row_id);
        }
    }
}

bool Equipment::checkLabel(str_t label_id)
{
    if (equipment_id=="1111"){
        return true;
    }
    for ( auto x=labels_list.begin(); x != labels_list.end();x++) {
if ( *x == label_id){
            return true;
        }
    }
    qInfo() << "Equipment::checkLabel: The label " << label_id <<" is not part of" << equipment_id <<"\n";
    return false;
}

PointerVector<BaseItem>& Equipment::getLabels()
{
    return labels;
}