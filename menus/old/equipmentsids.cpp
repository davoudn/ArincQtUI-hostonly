#include "equipmentsids.h"
#include "utils.h"

EquipmentsIds::EquipmentsIds(){
/*
 *
 */
    str_t jsonstr;
#ifdef WINDOWS
    jsonstr = utils::ReadFile("E:\\davoud\\projects\\ARINC-QT\\Source\\ArincQtUI-hostonly\\menus\\Labels\\equipment_names.json");
#endif
#ifdef RASBERRY
    jsonstr = utils::ReadFile("/home/pouyan/ARINC-QT/Source/ArincQtUI/menus/Labels/equipment_names.json");
#endif
    jsondoc_t jdoc = jsondoc_t::fromJson(jsonstr.toUtf8());
    jsonobj_t jobj = jdoc.object();

    for (auto& x : jobj.keys()) {
        str_t _nameAndId =  x + "-" + jobj[x].toString() ;
        IdToName[x] = _nameAndId;
        NameToId[_nameAndId] = x;
    }
}
//
str_t EquipmentsIds::FindId(str_t _name){
    auto _id = NameToId.find(_name);
    if (_id != NameToId.end()){
        return _id->second;
    }
    return "null";
}
//
str_t EquipmentsIds::FindName(str_t _id){
    auto _name = IdToName.find(_id);
    if (_name != IdToName.end()){
        return _name->second;
    }
    return "null";
}
//
std::vector<str_t> EquipmentsIds::GetNames(){
    std::vector<str_t> _listofnames;
    for (auto& x: NameToId){
        _listofnames.push_back(x.first);
    }
    return _listofnames;
}
