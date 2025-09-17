#ifndef LABEL_H
#define LABEL_H

#include "BaseParameter.h"
#include "types.h"
#include "baseitem.h"
#include "PointerVector.h"

#include <QTimer>
#include <vector>
#include <cstdlib>
#include <cstdint>
#include <vector>



str_t ReadFile(str_t filename);


class DataSourceBase;
class Equipment;
/* actuall label class
 * It only contains a list of parametrs (std::map<str_t, arinc_parameter_t>) and an arinc data (arinc_data_t).
 * by a GetValue function it could extract the data which is encoded into arinc_data_t.
 */
class Label: public BaseItem {
public:
    //
    Label(str_t _path, str_t equipmentId, str_t labelId);
   // Label() = default;
    ~Label();

    value_t valueEng(str_t pname, DArincData& data);
    value_t valueBits(str_t pname, DArincData& data);

    virtual value_t valueBits(str_t pname);
    virtual value_t valueEng(str_t pname);
    virtual value_t valueBits();

    virtual void setValueBits(value_t _bits);
    virtual void setValueEng(str_t _pname, value_t _value);

    // inlines ...
    inline DArincData& getArincData(){
        return ArincData;
    }

    inline bool getIfActive(){
        return bIfActive;
    }

    inline void setStatus(str_t _status){
        if (_status == "Active"){
            bIfActive = true;
        }

        if (_status == "Idle"){
            bIfActive = false;
        }
    }

    inline void   activate(bool _bIfActive){
       bIfActive = _bIfActive;
    }
    value_t getDataRate(){
        return value_t(dataRate);
    }

    virtual void setDataRate(value_t _value){
        dataRate = _value.toDouble();
    }
public:
    int init(str_t _path, str_t equipmentId, str_t labelId);
    void clear();
    int parseFromJson(const str_t& jsonData);

    BaseItem* getParameter(str_t p_name);
    const PointerVector<BaseItem>& getParameters();
    str_t getStatus();
    str_t getName();
    const std::vector<str_t>& getParametersNames();

    void setArincData(DArincData arincdata);
    void addParams(jsonobj_t);

    
    str_t Name;
    str_t EquipmentId;
    str_t LabelId;
    str_t Type;
    PointerVector<BaseItem> Params;
    std::vector<str_t> ParamsNames;

    bool bIfActive = true;
    str_t path;
    DArincData ArincData;
    double dataRate = 0.f;

};


/*                                  */
/* end of label definition */
/*                                  */

#endif // 
