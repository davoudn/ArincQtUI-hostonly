#ifndef TRANSMITTERWORKER_H
#define TRANSMITTERWORKER_H

#include "types.h"
#include "PointerVector.h"
#include "TreeDataModel.h"

#include <QObject>
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
class BaseAction;

class TransmitterWorker: public QObject {
    Q_OBJECT
    TransmitterWorker(str_t _equipment, int ch);
    TransmitterWorker() = delete;
    static TransmitterWorker* instance0;
    static TransmitterWorker* instance1;


public:
    virtual ~TransmitterWorker() override;
    static TransmitterWorker* getInstance(int ch);
    virtual void run();
     inline PointerVector<BaseItem>& getEquipments(){
         return equipments;
     }
     Equipment* getEquipment();
     void startTasks() ;
     int chanell = 0;
public slots:
     void taskTransmitData();
     void incrementLabelsDataRateCounter();
     void actionListCleaner();
 signals:
     bool sendData(BaseAction* ac);

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
