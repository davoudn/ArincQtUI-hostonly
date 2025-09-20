#ifndef RECEIVERWORKER_H
#define RECEIVERWORKER_H
#include "types.h"
#include "Timer.h"
#include "PointerVector.h"
#include "receiver.h"
#include "TreeDataModel.h"

#include <QThread>
#include <QtConcurrent/QtConcurrent>

template <typename T, int I>
class DeviceDelegate;

class DArincData;
class Equipment;
class BaseItem;
class transmitter;
class Receiver;

//class Receiver;
//class Transmitter;

#define DEVICEONE_CHANELLONE 11
#define DEVICEONE_CHANELLTWO 12



class ReceiverWorker: public QObject{
    Q_OBJECT
    ReceiverWorker(str_t _equipment, uint8_t ch);

    ReceiverWorker( ) = delete;
    static ReceiverWorker* instance0;
    static ReceiverWorker* instance1;
    static ReceiverWorker* instance2;
    static ReceiverWorker* instance3;


public:

    static ReceiverWorker* getInstance(uint8_t ch);

    void update(std::bitset<ARINC32_SIZE>&& _ArincBitData) ;
    void update(std::bitset<ARINC32_SIZE>&  _ArincBitData) ;
    void update(float _rate, std::bitset<ARINC32_SIZE>& arincBitsData);

     DArincData& getArincData();
     PointerVector<BaseItem>& getEquipments();
     Equipment* getEquipment();
     void startTasks();

    bool bIfDataUpdated = false;
    bool bIfDataHandled = true;
    bool bIfToClean = false;
    Receiver *parent=nullptr;
    DArincData* ArincData;
    float rate = 0.f;
public slots:
    void idleLabelCleaner();
    void evalDataRates();
    void receiveTask();

protected:

    PointerVector<BaseItem> equipments;
    str_t defaultEquipment;
    //QFuture <void> idleLabelCleanerFuture;
    Timer* idleLabelCleanerTimer = nullptr;
    Timer* dataRateTimer = nullptr;
    bool bIfEnabled = false;

    QThread* mainThread = nullptr;
    QThread* idlecleanerThread = nullptr;
    QThread* dataRateThread = nullptr;
    uint8_t chanell = 0;


};






#endif // RECEIVERWORKER_H



