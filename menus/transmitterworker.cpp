#include "transmitterworker.h"
#include "Label.h"
#include "Equipment.h"
#include "arinc_functions.h"
#include "transmitter.h"
#include "Timer.h"
#include "DEI1016.h"
#include "generaldata.h"
#include <thread>
#include <chrono>

TransmitterWorker* TransmitterWorker::instance = nullptr;



Equipment*  TransmitterWorker:: getEquipment()
{
    return static_cast<Equipment*>(equipments[0]);
}

TransmitterWorker* TransmitterWorker::getInstance()
{
    if (!instance){
        instance = new TransmitterWorker("002");
        return instance;
    }
    else {
        return instance;
    }
}

TransmitterWorker::TransmitterWorker(str_t _equipment)
{
    //
    defaultEquipment = "002";
    equipments.push_back(new Equipment(defaultEquipment,EquipmentRole::Transmitter));
    mainThread = new QThread();
    dataRateThread = new QThread();

    mainThread->setObjectName(" Main Transmitter thread... ");
    dataRateThread->setObjectName(" Transmitter data rate thread... ");

    dataRateTimer = new Timer(this);
    dataRateTimer->setTimeout(ACTION_CLEANER_TIME);

    connect(mainThread, &QThread::started, this, &TransmitterWorker::taskTransmitData);
    connect(dataRateThread, &QThread::started, dataRateTimer, &Timer::counterTask);
    //
    connect(dataRateTimer, &Timer::onTimeout, this, &TransmitterWorker::actionListCleaner,  Qt::DirectConnection);
    connect(this, &TransmitterWorker::sendData, DEI1016::getInstance(), &DEI1016::sendData, Qt::BlockingQueuedConnection);
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
    DEI1016::getInstance();
    if (!DEI1016::getInstance()->bIfSerialOpen){
        DEI1016::getInstance()->openSerialPort();
    }
    mainThread->start();
    dataRateThread->start();
}
/*
 * SLOTS
 */

void TransmitterWorker::actionListCleaner()
{
    QMutexLocker<QMutex> locker(&GeneralData::getInstance()->mutex);
    for (auto it = transmitter::getInstance()->getActions().begin();  it!=transmitter::getInstance()->getActions().end(); it++){
        if ( (*it).bIfApplied ){
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
     transmitter::getInstance()->incrementLabelsDataRateCounter();
}

void TransmitterWorker::taskTransmitData()
{
    qInfo() << "TransmitterWorker::taskTransmitData() is runnig on "<< QThread::currentThread();
    while(1){
        std::this_thread::sleep_for(std::chrono::microseconds(1));
        QMutexLocker<QMutex> mutexlocker(&GeneralData::getInstance()->mutex);
        for (auto& x: transmitter::getInstance()->getActions() )
        {
            if (!x.bIfApplied){
                std::this_thread::sleep_for(std::chrono::microseconds(1));
                emit sendData(x);
                qInfo() << x.chanel<<"\t"<<x.instruction<<"\t"<<x.arincData<< "\t" << x.bIfApplied;
            }
        }
    }
}

