#include "actionsrecord.h"


/*
 *
 *
*/
bool DataRecord::set(record_t& d){
    bool retval = false;
        if (mutex.tryLock())
        {
            if(!bIsPopulated){
                data = d;
                bIsPopulated = true;
                retval =  true;
            }
            else {
                retval = false;
            }
             mutex.unlock();
        }
    return retval;
}

std::pair<bool,record_t> DataRecord::get(){
    std::pair<bool,record_t> p(false,record_t());
    if (mutex.tryLock())
    {
        if (bIsPopulated) {
            bIsPopulated = false;
            p.first = true;
            p.second = data;
        }
        mutex.unlock();
    }
    return p;
}


/*
 *
 *
*/


ActionsRecords::ActionsRecords(int N):actions(N) {}

std::pair<bool,int> ActionsRecords::record (record_t& d){
    for (uint32_t i=0; i < actions.size(); i++){
        if (actions[i].set(d)){
            return std::make_pair(true,std::move(i));
        }
    }
    return std::make_pair(false,-1);
}


std::pair<bool,record_t> ActionsRecords::getAt(uint32_t i){
    if (i>0 && i<actions.size()){
        return actions[i].get();
        //qInfo() << " ActionsRecords::getAt";
    }
    return std::make_pair(false,record_t());
}

/*
 * receiver records
*/

ReceiverRecords* ReceiverRecords::getInstance()
{
    if (!instance){
        instance = new ReceiverRecords(RECEIVER_RECORDE_SIZE);
    }
    return instance;
}

ReceiverRecords* ReceiverRecords::instance = nullptr;

/*
 * transmitter records
*/

TransmitterRecords* TransmitterRecords::getInstance()
{
    if (!instance){
        instance = new TransmitterRecords(RECEIVER_RECORDE_SIZE);
    }
    return instance;
}

TransmitterRecords* TransmitterRecords::instance = nullptr;



