#ifndef ACTIONSRECORD_H
#define ACTIONSRECORD_H

#include "types.h"
#include <QMutex>
#include <vector>
#include <stdint.h>

struct DataRecord{
protected:
    bool bIsPopulated = false;
    record_t data;
    QMutex mutex;
public:
    bool isLocked();
    std::pair<bool,record_t> get();
    bool set(record_t&);
public:


    inline bool isPopulated(){
        return bIsPopulated;
    }
};



class ActionsRecords
{
protected:
    ActionsRecords(int N);
    ActionsRecords() = delete;

    std::vector<DataRecord> actions;
/* inlines */
public:
    std::pair<bool,record_t> getAt(uint32_t i);

    inline    uint32_t resize(uint32_t N){
        return actions.size();
    }
    uint32_t size()
    {
        return actions.size();
    }
    std::pair<bool,int> record (record_t& d);

};

class ReceiverRecords : public ActionsRecords
{
    ReceiverRecords(int N):ActionsRecords(N) {
    }
    static ReceiverRecords* instance;
public:
    static ReceiverRecords* getInstance();
};


#endif // ACTIONSRECORD_H
