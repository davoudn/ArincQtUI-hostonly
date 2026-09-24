#pragma once
#include "types.h"
#include "PointerVector.h"
#include "baseitem.h"

#include <map>

class QThread;
class DArincData;
class Label;

class Equipment: public BaseItem {
public:
    Equipment(str_t equipment_id, EquipmentRole role);
    Equipment() = delete;
    ~Equipment() = default;
    Label* findLabel(str_t label);
    str_t readName(str_t equipment_id);
    str_t loadName(str_t, str_t);
    str_t findLabelIdFromName(str_t id_and_name);
    bool checkLabel(str_t label);
    void init(str_t equipment_id);
    void clear();
    bool addReservedLabel();
    bool addLabel(str_t label_id);
    bool addLabel(str_t label_id, QThread* thread);
    void removeLabel(str_t label_id);
    void removeLabel(Label* label);
    void removeLabel(uint32_t row_id);
    void insertLabel(uint32_t index, str_t label_id);
    PointerVector<BaseItem>& getLabels();
    inline const std::map <str_t,str_t>& getLabelsIdAndNames(){
        return labels_id_and_names;
    }
    inline str_t getEquipmentIdAndName(){
        return equipment_id + "( " + equipment_name + " )";
    }
    inline str_t getEquipmentId(){
        return equipment_id;
    }

protected:
    str_t equipment_id;
    str_t equipment_name;
    std::vector <str_t> labels_list;
    std::map <str_t,str_t> labels_id_and_names;
    PointerVector <BaseItem> labels;
    str_t path;
    EquipmentRole role;
};