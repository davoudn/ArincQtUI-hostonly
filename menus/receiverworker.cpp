#include "receiverworker.h"

#include "ArincData.h"
#include "Label.h"
#include "Equipment.h"
#include "arinc_functions.h"
#include "baseitem.h"
#include "DEI1016.h"
#include "DEI1016RasberryConfigurations.h"


ReceiverWorker* ReceiverWorker::instance0 = nullptr;
ReceiverWorker* ReceiverWorker::instance1 = nullptr;
ReceiverWorker* ReceiverWorker::instance2 = nullptr;
ReceiverWorker* ReceiverWorker::instance3 = nullptr;


ReceiverWorker::ReceiverWorker(str_t _equipment, uint8_t ch):chanell(ch)
{
    //
    makeDeviceIndex();
    ArincData = new DArincData();
    equipments.push_back(new Equipment(_equipment, EquipmentRole::Receiver));
// threads
    mainThread = new QThread();
    mainThread->setObjectName("Main receiver thread...");

    idlecleanerThread = new QThread();
    idlecleanerThread->setObjectName("idlecleaner thread...");

    dataRateThread = new QThread();
    dataRateThread->setObjectName("datRate thread...");
// timers
     idleLabelCleanerTimer = new Timer();
     idleLabelCleanerTimer->setTimeout(CLEANING_CHECK_TIME);

     dataRateTimer = new Timer();
     dataRateTimer->setTimeout(DATA_RATE_EVAL_TIME);

      connect(idleLabelCleanerTimer, &Timer::onTimeout, this, &ReceiverWorker::idleLabelCleaner, Qt::DirectConnection);
      connect(dataRateTimer, &Timer::onTimeout, this, &ReceiverWorker::evalDataRates, Qt::DirectConnection);
      //
      connect(dataRateThread, &QThread::started, dataRateTimer, &Timer::counterTask);
      connect(DEI1016::getInstance(), &DEI1016::update, this, &ReceiverWorker::update, Qt::BlockingQueuedConnection);

     // connect(mainThread, &QThread::started, this, &ReceiverWorker::receiveTask);
      connect(idlecleanerThread, &QThread::started, idleLabelCleanerTimer, &Timer::counterTask);
      //
      dataRateTimer->moveToThread(dataRateThread);
      idleLabelCleanerTimer->moveToThread(idlecleanerThread);
      this->moveToThread(mainThread);
}
/*
 * SLOTS ....
 */

void ReceiverWorker::startTasks()
{
    if (!DEI1016::getInstance()->bIfSerialOpen){
        DEI1016::getInstance()->openSerialPort();
    }
//    dataRateThread->start();
    mainThread->start();
  //  idlecleanerThread->start();
}

void ReceiverWorker::idleLabelCleaner()
{
    bIfToClean = true;
}

void ReceiverWorker::evalDataRates()
{
    Receiver::getInstance(chanell)->evalDataRates(DATA_RATE_EVAL_TIME);
}

ReceiverWorker* ReceiverWorker::getInstance(uint8_t ch)
{
  /*  if (!instance0) {
        instance0 = new ReceiverWorker("002", CHANELL0);
        return instance0;
    }
    else {
        return instance0;
    }
*/
    switch(ch)
    {
        case CHANELL0: {
            if (!instance0) {
                instance0 = new ReceiverWorker("002", CHANELL0);
                return instance0;
            }
            else {
                return instance0;
            }
            break;
        }
        case CHANELL1: {
            if (!instance1) {
                instance1 = new ReceiverWorker("002", CHANELL1);
                return instance1;
            }
            else {
                return instance1;
            }
            break;
        }
        case CHANELL2: {
            if (!instance2) {
                instance2 = new ReceiverWorker("002", CHANELL2);
                return instance2;
            }
            else {
                return instance2;
            }
            break;
        }
        case CHANELL3: {
            if (!instance3) {
                instance3 = new ReceiverWorker("002", CHANELL3);
                return instance3;
            }
            else {
                return instance3;
            }
        break;
        }
    }
    return nullptr;
}


DArincData&  ReceiverWorker::getArincData()
{
    return *ArincData;
}

PointerVector<BaseItem>& ReceiverWorker:: getEquipments()
{
    return equipments;
}

Equipment*  ReceiverWorker::getEquipment()
{
    return static_cast<Equipment*>(equipments[0]);
}
#include <thread>
#include <chrono>
void ReceiverWorker::receiveTask()
{
    qInfo() <<"ReceiverWorker::run() : started on chanel: "<<chanell << ", running thread: " << QThread::currentThread() <<"\n";

    while(1)
    {
        std::this_thread::sleep_for(std::chrono::microseconds(100));
        if (bIfDataUpdated)
        {

            str_t labelid = getArincData().template Get<LabelIdOctal>().toString();
            value_t value = getArincData().template Get<DataBits>();
            Receiver::getInstance(chanell)->setLabelData(labelid, rate, value);
            //if ( ){
            bIfDataHandled = true;
            bIfDataUpdated = false;
            //}
            // qInfo() <<"Object: " <<this->metaObject()<<" ReceiverWorker::receiveTask() chanel: "<<chanell<<"\t"<<value<<"\n";

        }

        if (bIfToClean)
        {
            Receiver::getInstance(chanell)->idleLabelCleaner();
            bIfToClean = false;
        }

    }
}


/*
void ReceiverWorker::update(std::bitset<ARINC32_SIZE>& arincBitsData)
{

    // std::cout << "ReceiverWorker:: Data received!! \n";
    if (this->bIfDataHandled) {
        ArincData->UpdateData(arincBitsData);
        bIfDataUpdated = true;
        bIfDataHandled = false;
    }
    else {
        qDebug() << "Data is not handled!! \n";
    }
}


void ReceiverWorker::update(std::bitset<ARINC32_SIZE>&& arincBitsData)
{
       qInfo() << "ReceiverWorker:: Data received!! \n";
    if (this->bIfDataHandled) {
        ArincData->UpdateData(arincBitsData);
        bIfDataUpdated = true;
        bIfDataHandled = false;
    }
    else {
        qDebug() << "Data is not handled!! \n";
    }
}
            emit update(dei, chanell, rate, arincData);

*/

void ReceiverWorker::makeDeviceIndex()
{
    switch (chanell) {
    case 0:
        dei = 0;
        deiChanell = 0;
        break;
    case 1:
        dei = 0;
        deiChanell = 1;
        break;
    case 2:
        dei = 1;
        deiChanell = 0;
        break;
    case 3:
        dei = 1;
        deiChanell = 1;
        break;
    }
}

void ReceiverWorker::update(uint8_t& deiId, uint8_t& chanellId, float& _rate, std::bitset<ARINC32_SIZE>& arincBitsData)
{
    if (deiId==dei && chanellId==deiChanell) {
        ArincData->UpdateData(arincBitsData);
        rate = _rate;

        //qInfo() << ArincData->getBitSet().to_string();
        str_t labelid = getArincData().template Get<LabelIdOctal>().toString();
        value_t value = getArincData().template Get<DataBits>();
        Receiver::getInstance(chanell)->setLabelData(labelid, rate, value);
    }

}




/*
 *
 *
 *
 *
 */

