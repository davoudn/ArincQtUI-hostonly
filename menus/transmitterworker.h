#ifndef TRANSMITTERWORKER_H
#define TRANSMITTERWORKER_H

#include "types.h"
#include "PointerVector.h"
#include "TreeDataModel.h"

#ifdef RASBERRY
#include "DEI1016.h"
#endif

#include <functional>

#include <QThread>
#include <QtConcurrent/QtConcurrent>

template <typename T, int I>
class DeviceDelegate;
class DArincData;
class Equipment;
class BaseItem;
class transmitter;
class Receiver;
class Timer;

class TransmitterWorker: public QObject {
    Q_OBJECT
    TransmitterWorker(str_t _equipment);
    TransmitterWorker() = delete;
    static TransmitterWorker* instance;

public:
    virtual ~TransmitterWorker() override;
    static TransmitterWorker* getInstance();
    virtual void run();
     inline PointerVector<BaseItem>& getEquipments(){
         return equipments;
     }
     Equipment* getEquipment();
     void startTasks() ;

public slots:
     void taskTransmitData();
     void incrementLabelsDataRateCounter();
     void actionListCleaner();
protected:

    PointerVector<BaseItem> equipments;
    DArincData* ArincData;

    bool bIfDataUpdated = false;
    bool bIfDataHandled = true;
    str_t  defaultEquipment ;


    QThread* mainThread;
    QThread* dataRateThread;
    Timer*  dataRateTimer = nullptr;

};

#endif // TRANSMITTERWORKER_H
