#ifndef EQUIPMENTSIDS_H
#define EQUIPMENTSIDS_H

#include "types.h"

#include <string>
#include <map>
#include <vector>

class EquipmentsIds {
public:
    EquipmentsIds();
    inline const std::map<str_t, str_t>& getIdToNameMap(){
        return IdToName;
    }
    //
    inline const std::map<str_t, str_t>& getNameToIdMap(){
        return NameToId;
    }
    //
    str_t findId(str_t x);
    str_t findName(str_t x);
    std::vector<str_t> getNames();

protected:
    std::map<str_t, str_t> IdToName;
    std::map<str_t, str_t> NameToId;
};

#endif // EQUIPMENTSIDS_H
