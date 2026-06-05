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
str_t EquipmentsIds::findId(str_t _name)
{
    auto _id = nameToId.find(_name);
    if (_id != nameToId.end()){
        return _id->second;
    }
    return "null";
}
//
str_t EquipmentsIds::findName(str_t _id)
{
    auto _name = idToName.find(_id);
    if (_name != idToName.end()){
        return _name->second;
    }
    return "null";
}
//
std::vector<str_t> EquipmentsIds::getNames()
{
    std::vector<str_t> _listofnames;
    for (auto& x: nameToId){
        _listofnames.push_back(x.first);
    }
    return _listofnames;
}
