#include "Equipment.h"
#include "ArincData.h"
#include "arinc_functions.h"
#include "receiver.h"
#include "utils.h"
#include "labelfor.h"


void Log(Label* label, DArincData& ArincData) {
    if (label) {
        qDebug()<< "Label Id: " << label->LabelId << "Label Name: " << label->Name << " Label Type:" << label->Type << "/*\n";
        for (auto& p : label->getParametersNames()) {
            qDebug() << "Parameter Name: " << p << " Engineering Value: " << label->valueEng(p, ArincData) << "\n";
            qDebug() << "Parameter Name: " << p << " Raw Value        : " << label->valueBits(p, ArincData) << "\n";
        }
        qDebug() << "*/\n";
    }
    else {
        qDebug() << "Label Id: " << ArincData.Get<LabelIdOctal>() << "is not part of this equipment, the raw value is: /* \n";
        qDebug() << " Value: " << ArincData.Get<DataBits>() << "\n";
    }
}


str_t Equipment::readName(str_t equimentId){
    jsonobj_t jobj = utils::GetJsonObj(path+ "equipment_names.json");

    if (jobj.contains(equimentId) ){
        return jobj[equimentId].toString();
    }

    return str_t("Unknown_Name");

}

PointerVector<BaseItem>& Equipment::GetLabels(){
    return labels;
}



Equipment::Equipment(str_t _equipmentId, EquipmentRole _role):BaseItem(BaseItem::ItemType::Equipment) {
#ifdef WINDOWS
    path    = "E:\\davoud\\projects\\ARINC-QT\\Source\\ArincQtUI-hostonly\\menus\\Labels\\";
#endif

#ifdef RASBERRY
    path    = "/home/pouyan/ARINC-QT/Source/ArincQtUI/menus/Labels/";
#endif
    role = _role;
    init(_equipmentId);
}

str_t Equipment::loadName(str_t _labelId, str_t _equipmentId){
    str_t filename = path + _labelId + "_" + _equipmentId + ".json";

    auto jobj = utils::GetJsonObj(filename);
    if (!Assert<Label>(jobj)){
        qDebug() << "Equipment::loadName: Label assert Failed. Data: "<< filename <<"\n";
        exit(1);
    }
    str_t tmp = _labelId + "( " + jobj["Name"].toString() + " )";
    return tmp;
}

void Equipment::init (str_t _equimentId) {
    clear();
    equipmentId    = _equimentId;
    equipmentName  = readName(equipmentId);
    auto jobj      = utils::GetJsonObj(path+"equipment.json");

    if (jobj.contains(equipmentId) ){
        int c = 0;
        for (const auto& labelId: jobj[equipmentId].toArray()){
            labelsList.push_back(labelId.toString());
            labelsIdAndNames.emplace( loadName(labelId.toString(), equipmentId), labelId.toString());
            c++;
        }
    }
    else {
        qDebug() << "Equipment::init  : Equipment Id was not found. Only Raw values will be shown. \n";
    }
    //
  //  qDebug() << "Equipment::init : Labels list:  \n";
    for (auto it = labelsIdAndNames.cbegin(); it != labelsIdAndNames.cend(); it++ ){
    //    addLabel(*it);
  //      std::cout << it->first.toStdString() << "-->" << it->second.toStdString() << "\n";
    }
}


Label* Equipment::findLabel(str_t _labelid) {
    for (auto& x: labels ){
        if (static_cast<Label*>(x)->LabelId == _labelid){
            return static_cast<Label*>(x);
        }
    }
    return nullptr;
}

str_t Equipment::findLabelIdFromName(str_t _idandname){
    auto x = labelsIdAndNames.find(_idandname);
    if (x!=labelsIdAndNames.end()){
        return x->second;
    }
    return "null";
}

void Equipment::clear(){
    labels.clear();
    labelsList.clear();
    labelsIdAndNames.clear();
}

bool Equipment::addLabel(str_t _labelId){
    Label* tmp = nullptr;
    if (checkLabel(_labelId)){
        if (role == EquipmentRole::Receiver){
            tmp = new LabelFor<DReceiver>(path, equipmentId, _labelId);
        }
        if (role == EquipmentRole::Transmitter){
            tmp = new LabelFor<DTransmitter>(path, equipmentId, _labelId);
        }
        tmp->parent = this;
        labels.push_back(tmp);
        return true;
    }
    return false;
}

void Equipment::insertLabel(uint32_t _position, str_t _labelId){
    if (checkLabel(_labelId)){
        Label* tmp = new Label(path,equipmentId,_labelId);
        tmp->parent = this;
        uint32_t c=0;
        for (auto it=labels.begin(); it!=labels.end();it++,c++){
            if (c==_position){
                labels.insert(it,tmp);
            }
        }
    }
}

void Equipment::removeLabel(str_t _labelId){
    Label* tmp = nullptr;
    for (auto& x: labels){
        if (static_cast<Label*>(x)->LabelId == _labelId){
            tmp = static_cast<Label*>(x);
        }
    }
    if (tmp){
        labels.remove(tmp);
    }
}

void Equipment::removeLabel(Label*  _label){
        labels.remove(_label);
}

void Equipment::removeLabel(uint32_t _rowid){
    if (_rowid>0 && _rowid<labels.size()){
        Label* tmp = static_cast<Label*>(labels[_rowid]);
        if (tmp){
            labels.remove(tmp);
        }
    }
}


bool Equipment::checkLabel(str_t _labelid){
     for ( auto x=labelsList.begin(); x != labelsList.end();x++) {
        if ( *x == _labelid){
         return true;
        }
     }
     qDebug() << "Equipment::findLabel: The label " << _labelid <<" is not part of" << equipmentId <<"\n";
    return false;
}


