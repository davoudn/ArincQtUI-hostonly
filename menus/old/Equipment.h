#include "types.h"
#include "PointerVector.h"
#include "baseitem.h"
#include <map>

class DArincData;
class Label;

class Equipment: public BaseItem {
public:
    Equipment(str_t equipmentID, EquipmentRole _role) ;
    Equipment() = delete;
    ~Equipment() = default;
    //
    Label*findLabel(str_t _label);
    str_t readName(str_t _equimentId);
    str_t loadName(str_t, str_t);
    str_t findLabelIdFromName(str_t _idandname);
    bool  checkLabel(str_t _label);
    void  init(str_t equipmentId);
    void  clear();
    bool  addLabel(str_t _labelid);
    void  removeLabel(str_t _labelid);
    void  removeLabel(Label*  _label);
    void  removeLabel(uint32_t _rowid);
    void  insertLabel(uint32_t _index, str_t _labelId);
    //
    PointerVector<BaseItem>& GetLabels();
    inline const std::map <str_t,str_t>& getLabelsIdAndNames(){
        return labelsIdAndNames;
    }
public:

    inline str_t getEqipmentIdAndName(){
        return equipmentId + "( " + equipmentName + " )";
    }

protected:
    str_t equipmentId;
    str_t equipmentName;
    std::vector <str_t> labelsList;
    std::map <str_t,str_t> labelsIdAndNames;
    PointerVector <BaseItem> labels;
    str_t path;
    EquipmentRole role;
};
