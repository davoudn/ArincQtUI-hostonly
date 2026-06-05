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
class DArincData;
/*
 * actuall label class
 * It only contains a list of parametrs (std::map<str_t, arinc_parameter_t>) and an arinc data (arinc_data_t).
 * by a valueXXX function it could extract the data which is encoded into arinc_data_t in the form of desiered representation.
 */
class Label: public BaseItem {
public:
    Label(str_t path, str_t equipment_id, str_t label_id);
    Label() = default;
    virtual ~Label() override;
    virtual str_t getStatus() const;
    virtual str_t getName() const;
    virtual value_t getValueBits() const ;
    virtual value_t getValueBits(str_t pname) const;
    virtual value_t getValueEng(str_t pname) const;
    virtual value_t getDataRate() const;
    DArincData& getArincData() const;
    uint32_t getUIntArincData() const;
    bool getIfActive() const;
    virtual void setValueBits(value_t bits);
    virtual void setValueEng(str_t pname, value_t value);
    virtual void timerCallback() ;
    virtual void setDataRate(value_t value);
    virtual void resetDataRateCounter();
    virtual void incrementDataRateCounter();
    void clear();
    void setStatus(str_t status);
    void activate(bool active);
    void setArincData(DArincData arinc_data);
    void addParams(jsonobj_t params);
    int init(str_t path, str_t equipment_id, str_t label_id);
    int initFromDatabase(str_t path, str_t equipment_id, str_t label_id);
    int parseFromJson(const str_t& json_data);
    BaseItem* getParameter(str_t p_name);
    const PointerVector<BaseItem>& getParameters() const;
    const std::vector<str_t>& getParametersNames() const;
public:
    str_t name;
    str_t equipment_id;
    str_t label_id;
    str_t type;
    PointerVector<BaseItem> params;
    std::vector<str_t> params_names;
protected:
    bool if_active = false;
    str_t path;
    DArincData arinc_data;
    double data_rate = 5.f;
    uint32_t time_out_counter = 0;
    uint32_t data_rate_counter = 0;
};


/*                                  */
/* end of label definition */
/*                                  */

#endif // LABEL_H