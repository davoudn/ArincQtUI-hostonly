#ifndef EQUIPMENTSIDS_H
#define EQUIPMENTSIDS_H

#include "types.h"

#include <string>
#include <map>
#include <vector>

class EquipmentsIds {
public:
    EquipmentsIds();
    inline const std::map<str_t, str_t>& GetIdToNameMap(){
        return IdToName;
    }
    //
    inline const std::map<str_t, str_t>& GetNameToIdMap(){
        return NameToId;
    }
    //
    str_t FindId(str_t x);
    str_t FindName(str_t x);
    std::vector<str_t> GetNames();

protected:
    std::map<str_t, str_t> IdToName;
    std::map<str_t, str_t> NameToId;
};

#endif // EQUIPMENTSIDS_H
