#include "equipmentsids.h"
#include "utils.h"
#include "generaldata.h"

EquipmentsIds::EquipmentsIds()
{
/*
 *
 */
    str_t jsonstr;

    jsonstr = utils::ReadFile(GeneralData::getInstance()->LABELS_PATH+ QString("equipment_names.json"));
    jsondoc_t jdoc = jsondoc_t::fromJson(jsonstr.toUtf8());
    jsonobj_t jobj = jdoc.object();

    for (auto& x : jobj.keys()) {
        str_t _nameAndId =  x + "-" + jobj[x].toString() ;
        IdToName[x] = _nameAndId;
        NameToId[_nameAndId] = x;
    }
}
//
str_t EquipmentsIds::FindId(str_t _name)
{
    auto _id = NameToId.find(_name);
    if (_id != NameToId.end()){
        return _id->second;
    }
    return "null";
}
//
str_t EquipmentsIds::FindName(str_t _id)
{
    auto _name = IdToName.find(_id);
    if (_name != IdToName.end()){
        return _name->second;
    }
    return "null";
}
//
std::vector<str_t> EquipmentsIds::GetNames()
{
    std::vector<str_t> _listofnames;
    for (auto& x: NameToId){
        _listofnames.push_back(x.first);
    }
    return _listofnames;
}
