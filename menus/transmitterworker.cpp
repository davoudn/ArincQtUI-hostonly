#include "transmitterworker.h"
#include "Label.h"
#include "Equipment.h"
#include "arinc_functions.h"
#include "transmitter.h"
#include "Timer.h"
#include "DEI1016.h"
#include <thread>
#include <chrono>
#include "types.h"
#include "DEI1016RasberryConfigurations.h"
#include "generaldata.h"
#include "action.h"
#include "actionsrecord.h"
#include "bitutils.h"

TransmitterWorker* TransmitterWorker::instance0 = nullptr;
TransmitterWorker* TransmitterWorker::instance1 = nullptr;



Equipment*  TransmitterWorker:: getEquipment()
{
    return static_cast<Equipment*>(equipments[0]);
}

TransmitterWorker* TransmitterWorker::getInstance(int ch)
{
    switch(ch){
        case CHANELL0:
        {
            if (!instance0){
                instance0 = new TransmitterWorker("002", ch);
                return instance0;
            }
            else {
                return instance0;
            }
        }
        case CHANELL1:
        {
            if (!instance1){
                instance1 = new TransmitterWorker("002", ch);
                return instance1;
            }
            else {
                return instance1;
            }
        }
    }
    return nullptr;
}

TransmitterWorker::TransmitterWorker(str_t _equipment, int ch)
{
    //
    chanell = ch;
    defaultEquipment = "002";
    equipments.push_back(new Equipment(defaultEquipment,EquipmentRole::Transmitter));
    mainThread = new QThread();
    dataRateThread = new QThread();
    mainThread->setObjectName(" Main Transmitter thread... ");
    dataRateThread->setObjectName(" Transmitter data rate thread... ");
    dataRateTimer = new Timer(this);
    dataRateTimer->setTimeout(ACTION_CLEANER_TIME);
    //
    connect(mainThread, &QThread::started, this, &TransmitterWorker::taskTransmitData);
    connect(dataRateThread, &QThread::started, dataRateTimer, &Timer::counterTask);
    //
    connect(dataRateTimer, &Timer::onTimeout, this, &TransmitterWorker::actionListCleaner,  Qt::DirectConnection);
    connect(this, &TransmitterWorker::sendData, DEI1016::getInstance(), &DEI1016::sendData, Qt::DirectConnection);

    //
    dataRateTimer->moveToThread(dataRateThread);
    this->moveToThread(mainThread);

}

TransmitterWorker::~TransmitterWorker()
{
 //   mainThread->quit();
 //   dataRateThread->quit();
   // delete mainThread;
  //  delete dataRateThread;
}

void TransmitterWorker::startTasks()
{
    mainThread->start();
    // dataRateThread->start();
}
/*
 * SLOTS
 */

void TransmitterWorker::actionListCleaner()
{
    QMutexLocker locker(&GeneralData::getInstance()->mutex);
    for (auto it = GeneralData::getInstance()->getActions().begin();  it!=GeneralData::getInstance()->getActions().end(); it++){
        if ( (*it)->bIfApplied ){
          //  transmitter::getInstance()->getActions().erase(it);
        }
    }
}

void TransmitterWorker::run()
{
    taskTransmitData();
}

void TransmitterWorker::incrementLabelsDataRateCounter()
{
    // transmitter::getInstance()->incrementLabelsDataRateCounter();
}

void TransmitterWorker::taskTransmitData()
{
    qInfo() << "TransmitterWorker::taskTransmitData() is runnig on "<< QThread::currentThread();
    char txBuff[FRAME_POCKET_SIZE];
    while(1)
    {
        std::this_thread::sleep_for(std::chrono::microseconds(10));
        for(uint32_t i=0; i < TransmitterRecords::getInstance()->size(); i++)
        {
            auto d = TransmitterRecords::getInstance()->getAt(i);
            if (d.first)
            {
              //  AUX::log(d.second,"transmit Data");
                for (int i=0; i < FRAME_POCKET_SIZE; i++){
                    txBuff[i] = d.second[i];
                }
                emit sendData(txBuff);
            }
        }
    }
}


/*
        // std::this_thread::sleep_for(std::chrono::microseconds(10));
        QMutexLocker mutexlocker(&GeneralData::getInstance()->mutex);
        for (int i=0; i < GeneralData::getInstance()->getActions().size(); i++)
        {
            if (!GeneralData::getInstance()->getActions()[i]->bIfApplied){
                qInfo() << i << "\t" << (GeneralData::getInstance()->getActions()[i]->bIfApplied);

               // std::this_thread::sleep_for(std::chrono::microseconds(1));
               emit sendData(GeneralData::getInstance()->getActions()[i]->toPacket());
               GeneralData::getInstance()->getActions()[i]->bIfApplied = true;
            }
        }
 */
