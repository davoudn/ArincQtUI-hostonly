#include "receiverworker.h"

#include "ArincData.h"
#include "Label.h"
#include "Equipment.h"
#include "arinc_functions.h"
#include "baseitem.h"
#include "DEI1016.h"
#include "DEI1016RasberryConfigurations.h"
#include "actionsrecord.h"
#include "bitutils.h"
#include "receiver.h"
#include "TreeDataModel.h"

ReceiverWorker* ReceiverWorker::instance0 = nullptr;
ReceiverWorker* ReceiverWorker::instance1 = nullptr;
ReceiverWorker* ReceiverWorker::instance2 = nullptr;
ReceiverWorker* ReceiverWorker::instance3 = nullptr;


ReceiverWorker::ReceiverWorker(str_t _equipment, uint8_t ch):chanell(ch)
{
    //
    makeDeviceIndex();
    equipments.push_back(new Equipment(_equipment, EquipmentRole::Receiver));

    mainThread.setObjectName("Main receiver thread...");
   // idlecleanerThread.setObjectName("idlecleaner thread...");
   // dataRateThread.setObjectName("datRate thread...");



    connect(&mainThread, &QThread::started, this, &ReceiverWorker::receiveTask);
    //connect(this, &ReceiverWorker::evalDataRates, Receiver::getInstance(chanell)->getDataModel(), &MyDataModel::evalDataRates, Qt::QueuedConnection);
    //connect(this, &ReceiverWorker::idleLabelCleaner, Receiver::getInstance(chanell)->getDataModel(), &MyDataModel::cleanTimeoutList, Qt::QueuedConnection);

    this->moveToThread(&mainThread);
    //idleLabelCleanerTimer.moveToThread(&mainThread);
    //dataRateTimer.moveToThread(&mainThread);

   // idleLabelCleanerTimer.start(CLEANING_CHECK_TIME);
   // dataRateTimer.start(DATA_RATE_EVAL_TIME);

}
/*
 * SLOTS ....
 */

void ReceiverWorker::startTasks()
{

    mainThread.start();

}


ReceiverWorker* ReceiverWorker::getInstance(uint8_t ch)
{

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
    dword_t arincBits;
    DArincData arincData;
    uint8_t de=0;
    uint8_t ch = 0;
    while(1)
    {
        std::this_thread::sleep_for(std::chrono::microseconds(10));
        for(uint32_t i=0; i < ReceiverRecords::getInstance()->size(); i++)
        {
            auto d = ReceiverRecords::getInstance()->getAt(i);
            if (d.first)
            {
             //   AUX::log(d.second,"received Data");
                AUX::convertBytesToData(d.second, de, ch, rate, arincBits);
                AUX::convertFromDEIToArinc(arincBits);
                arincData.Init(arincBits);
                str_t labelid = arincData.template Get<LabelIdOctal>().toString();
                value_t value = arincData.template Get<DataBits>();
                auto res = QtConcurrent::run( [=](){
                       emit setLabelData(de,ch,labelid, rate, value);//});
                });
            }
        }
       if (bIfToClean)
        {
           auto res = QtConcurrent::run( [=](){
                  emit idleLabelCleaner();//});
           });
            bIfToClean = false;
        }
        if (bIfEvalDatarates)
        {
            auto res = QtConcurrent::run( [=](){
                   emit evalDataRates();//});
            });
            bIfEvalDatarates = false;
        }

    }
}

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
        bIfDataUpdated = true;
    }

}

void  ReceiverWorker::enableEvalrates()
{
     bIfEvalDatarates = true;
}
void  ReceiverWorker::enableIdleCleaner()
{
    bIfToClean = true;
}
