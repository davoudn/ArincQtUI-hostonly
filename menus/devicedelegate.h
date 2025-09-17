#ifndef DEVICEDELEGATE_H
#define DEVICEDELEGATE_H
#include <functional>
#include "types.h"
#include "Timer.h"
#include "PointerVector.h"
#include "receiver.h"
#include "transmitter.h"
#include "TreeDataModel.h"

#ifdef RASBERRY
#include "DEI1016.h"
#endif

#include <QThread>
#include <QtConcurrent/QtConcurrent>

template <typename T, int I>
class DeviceDelegate;

class DArincData;
class Equipment;

class BaseItem;

//class Receiver;
//class Transmitter;

#define DEVICEONE_CHANELLONE 11
#define DEVICEONE_CHANELLTWO 12

template <typename T>
class DataHandlerTask: public QThread {
public:
    DataHandlerTask(T* _parent);
    void run() override;
    bool bIfRun = true;
private:
    T* parent =nullptr;
};


class BaseDevice : public QThread{
  public :
    BaseDevice() = default;

    virtual void timerCallback(){}
};


template <int DEVICEINDEX>
class DeviceDelegate<DReceiver, DEVICEINDEX>: public BaseDevice{
    DeviceDelegate(str_t _equipment);

    DeviceDelegate( ) = delete;
    static DeviceDelegate<DReceiver, DEVICEINDEX>* instance;

public:

    static DeviceDelegate<DReceiver, DEVICEINDEX>* getInstance();

    void update(std::bitset<ARINC32_SIZE>&& _ArincBitData) ;
    void update(std::bitset<ARINC32_SIZE>&  _ArincBitData) ;
    void run() override;
    void timerCallback() override;

     DArincData& getArincData();
     PointerVector<BaseItem>& getEquipments();
     Equipment* getEquipment();

    bool bIfDataUpdated = false;
    bool bIfDataHandled = true;
    Receiver *parent=nullptr;

    DArincData* ArincData;

public slots:
    void idleLabelCleaner();
    void dataRateEvaluator();

protected:

    PointerVector<BaseItem> equipments;
    str_t defaultEquipment;
    //QFuture <void> idleLabelCleanerFuture;
    QTimer* idleLabelCleanerTimer = nullptr;
     Timer* dataRateTimer = nullptr;

    using datahandler_t = DataHandlerTask <DeviceDelegate<DReceiver, DEVICEINDEX>>;
};


#include "ArincData.h"
#include "Label.h"
#include "Equipment.h"
#include "arinc_functions.h"
#include "baseitem.h"

template <int DEVICEINDEX>
DeviceDelegate<DReceiver, DEVICEINDEX>::DeviceDelegate(str_t _equipment){
    //
    ArincData = new DArincData();
    equipments.push_back(new Equipment(_equipment, EquipmentRole::Receiver));

    // do not start main task here this device  DATA_RATE_EVAL_TIME
    idleLabelCleanerTimer = new QTimer(this);
    connect(idleLabelCleanerTimer, &QTimer::timeout, this, &DeviceDelegate<DReceiver, DEVICEINDEX>::idleLabelCleaner);
    idleLabelCleanerTimer->start(CLEANING_CHECK_TIME);

    //connect(idleLabelCleanerTimer, &QTimer::timeout, this, &DeviceDelegate<DReceiver, DEVICEINDEX>::dataRateEvaluator);

     dataRateTimer = new Timer(this);
     dataRateTimer->setTimeout(DATA_RATE_EVAL_TIME);
   //  dataRateTimer->start();
}
/*
 * SLOTS ....
 */

template <int DEVICEINDEX>
void DeviceDelegate<DReceiver,DEVICEINDEX>::idleLabelCleaner(){

    auto _dataModel = Receiver::getInstance()->getDataModel();
    if (_dataModel){
        _dataModel->cleanTimeoutList();
    }
}

template <int DEVICEINDEX>
void DeviceDelegate<DReceiver,DEVICEINDEX>::dataRateEvaluator(){

    auto _dataModel = Receiver::getInstance()->getDataModel();
    if (_dataModel){
        _dataModel->evalDataRates(DATA_RATE_EVAL_TIME);
    }
}

template <int DEVICEINDEX>
void DeviceDelegate<DReceiver,DEVICEINDEX>:: timerCallback() {
    dataRateEvaluator();
}


/*
 *
 */


template <int DEVICEINDEX>
DeviceDelegate<DReceiver, DEVICEINDEX>* DeviceDelegate<DReceiver,DEVICEINDEX>::getInstance(){
    if (!instance) {
        instance = new DeviceDelegate<DReceiver, DEVICEINDEX>("002");
        return instance;
    }
    else {
        return instance;
    }
}

template <int DEVICEINDEX>
 DArincData&  DeviceDelegate<DReceiver, DEVICEINDEX>::getArincData(){
    return *ArincData;
}

template <int DEVICEINDEX>
 PointerVector<BaseItem>& DeviceDelegate<DReceiver,DEVICEINDEX>:: getEquipments(){
    return equipments;
}

template <int DEVICEINDEX>
 Equipment*  DeviceDelegate<DReceiver,DEVICEINDEX>::getEquipment(){
    return static_cast<Equipment*>(equipments[0]);
}

template <int DEVICEINDEX>
void DeviceDelegate<DReceiver, DEVICEINDEX>::run(){
    std::cout <<"DeviceDelegate<DReceiver, DEVICEINDEX>::run() : started .........\n";
    while(1) {
        if (bIfDataUpdated) {
            //
            str_t labelid = getArincData().template Get<LabelIdOctal>().toString();
            value_t value = getArincData().template Get<DataBits>();
          //  qDebug() << "labelid: "  <<labelid<<" Data: " <<  value << "\n";
            auto _dataModel = Receiver::getInstance()->getDataModel();
            if (_dataModel){
                //_dataModel->setLabelData(labelid, value);
                _dataModel->setLabelData(labelid, value, this);
            }

            bIfDataHandled = true;
            bIfDataUpdated = false;
        }

    }
}


template <int DEVICEINDEX>
void DeviceDelegate<DReceiver,DEVICEINDEX>::update(std::bitset<ARINC32_SIZE>& arincBitsData)  {
    /*
     *  it should be very low latency handling the actual handling must be done through a task
     */
    // std::cout << "DeviceDelegate<DReceiver,DEVICEINDEX>:: Data received!! \n";
    if (this->bIfDataHandled) {
        ArincData->UpdateData(arincBitsData);
        bIfDataUpdated = true;
        bIfDataHandled = false;
    }
    else {
        qDebug() << "Data is not handled!! \n";
    }
}

template <int DEVICEINDEX>
void DeviceDelegate<DReceiver, DEVICEINDEX>::update(std::bitset<ARINC32_SIZE>&& arincBitsData)  {
    /*
     *  it should be very low latency handling the actual handling must be done through a task
     */
       std::cout << "DeviceDelegate<DReceiver,DEVICEINDEX>:: Data received!! \n";
    if (this->bIfDataHandled) {
        ArincData->UpdateData(arincBitsData);
        bIfDataUpdated = true;
        bIfDataHandled = false;
    }
    else {
        qDebug() << "Data is not handled!! \n";
    }
}

/*
 *
 *
 *
 *
 *
 *
 *  Transmiter delegate singletone
 *
 *
 *
 *
 *
 *
 *
 */

template <int DEVICEINDEX>
class DeviceDelegate<DTransmitter, DEVICEINDEX>: public BaseDevice {
    DeviceDelegate(str_t _equipment);
    DeviceDelegate( ) = delete;
    static DeviceDelegate<DTransmitter, DEVICEINDEX>* instance;

    virtual void run() override;
    virtual void timerCallback() override;

public:

    static DeviceDelegate<DTransmitter, DEVICEINDEX>* getInstance();

    void startHandlingData();

     inline PointerVector<BaseItem>& getEquipments(){
         return equipments;
     }

     Equipment* getEquipment();
     void incrementLabelsDataRateCounter();

protected:

    PointerVector<BaseItem> equipments;
    DArincData* ArincData;
     Timer* dataRateTimer = nullptr;

    bool bIfDataUpdated = false;
    bool bIfDataHandled = true;

    str_t  defaultEquipment ;
};




template <int DEVICEINDEX>
Equipment*  DeviceDelegate<DTransmitter,DEVICEINDEX>:: getEquipment(){
    return static_cast<Equipment*>(equipments[0]);
}

template <int DEVICEINDEX>
DeviceDelegate<DTransmitter, DEVICEINDEX>* DeviceDelegate<DTransmitter,DEVICEINDEX>::getInstance(){
    if (!instance){
        instance = new DeviceDelegate<DTransmitter, DEVICEINDEX>("002");
        return instance;
    }
    else {
        return instance;
    }
}




template <int DEVICEINDEX>
DeviceDelegate<DTransmitter, DEVICEINDEX>::DeviceDelegate(str_t _equipment){
    //
    defaultEquipment = "002";
    equipments.push_back(new Equipment(defaultEquipment,EquipmentRole::Transmitter));

    dataRateTimer = new Timer(this);
    dataRateTimer->setTimeout(MIN_TICK);
   // dataRateTimer->start();
    // setup time for transmmiter data rates
   // dataRateTimer = new QTimer(this);
   // connect(dataRateTimer, &QTimer::timeout, this, &DeviceDelegate<DTransmitter, DEVICEINDEX>::incrementLabelsDataRateCounter);
   // dataRateTimer->start(TRANSMITTER_INTERVAL_TIME);


}

template <int DEVICEINDEX>
void DeviceDelegate<DTransmitter, DEVICEINDEX>::run(){
    std::cout << "DeviceDelegate<DTransmitter, DEVICEINDEX>::run() : sending thread was started."<<std::endl;
    while(1){
        auto _dataModel = transmitter::getInstance()->getDataModel();
        if (_dataModel){
            std::vector<DArincData> dataToSend =_dataModel->getListOfAvailableLabelData();
            for (auto& x: dataToSend ){
               // qDebug() << x.Get<LabelIdOctal>() << "\n";
//#ifdef RASBERRY
                 DEI1016::getInstance()->SetSendBuff(0, x.getBitSet());
                 DEI1016::getInstance()->SendData(0);
//#endif
            }
        }
    }
}
/*
 * SLOTS
 */
template <int DEVICEINDEX>
void DeviceDelegate<DTransmitter, DEVICEINDEX>::timerCallback(){
    incrementLabelsDataRateCounter();
}
template <int DEVICEINDEX>
void DeviceDelegate<DTransmitter, DEVICEINDEX>::incrementLabelsDataRateCounter(){
  auto _dataModel = transmitter::getInstance()->getDataModel();
  // qDebug() << " DeviceDelegate<DTransmitter, DEVICEINDEX>::incrementLabelsDataRateCounter :: increment..." << "\n";;

  if(_dataModel){
    _dataModel->incrementLabelsDataRateCounter();
  }
}

#endif // DEVICEDELEGATE_H



