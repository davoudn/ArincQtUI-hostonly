#ifndef LABEL_H
#define LABEL_H

#include "BaseParameter.h"
#include "types.h"
#include "baseitem.h"
#include "PointerVector.h"
#include  "Timer.h"


#include <QTimer>
#include <vector>
#include <cstdlib>
#include <cstdint>
#include <vector>



str_t ReadFile(str_t filename);


class DataSourceBase;
class Equipment;
/*
 * actuall label class
 * It only contains a list of parametrs (std::map<str_t, arinc_parameter_t>) and an arinc data (arinc_data_t).
 * by a valueXXX function it could extract the data which is encoded into arinc_data_t in the form of desiered representation.
 */
class Label: public BaseItem {
public:
    //
    Label(str_t _path, str_t equipmentId, str_t labelId);
   // Label() = default;
    virtual ~Label() override;

    value_t valueEng(str_t pname, DArincData& data);
    value_t valueBits(str_t pname, DArincData& data);

    virtual value_t valueBits(str_t pname);
    virtual value_t valueEng(str_t pname);
    virtual value_t valueBits();

    virtual void setValueBits(value_t _bits);
    virtual void setValueEng(str_t _pname, value_t _value);
    virtual void timerCallback() ;

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


    virtual void setDataRate(value_t _value){
        dataRate = _value.toDouble();
    }

    virtual void resetDataRateCounter(){
        dataRateCounter = 0;
    }

    virtual void incrementDataRateCounter(){
        dataRateCounter++;
    }

    virtual inline value_t getDataRate(){
        return value_t(dataRate);
    }
public:

    int init(str_t _path, str_t equipmentId, str_t labelId);
    void clear();
    int parseFromJson(const str_t& jsonData);

    BaseItem* getParameter(str_t p_name);
    const PointerVector<BaseItem>& getParameters();
    virtual str_t getStatus();
    virtual str_t getName();
    const std::vector<str_t>& getParametersNames();

    void setArincData(DArincData arincdata);
    void addParams(jsonobj_t);

    
    str_t Name;
    str_t EquipmentId;
    str_t LabelId;
    str_t Type;
    PointerVector<BaseItem> Params;
    std::vector<str_t> ParamsNames;

protected:

    bool bIfActive = true;
    str_t path;
    DArincData ArincData;
    double dataRate = 5.f;
    uint32_t timeOutCounter = 0;
    uint32_t dataRateCounter = 0;

};


/*                                  */
/* end of label definition */
/*                                  */

#endif // 
