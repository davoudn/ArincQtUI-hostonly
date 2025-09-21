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
    for (auto it = transmitter::getInstance(chanell)->getActions().begin();  it!=transmitter::getInstance(chanell)->getActions().end(); it++){
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
    // transmitter::getInstance()->incrementLabelsDataRateCounter();
}

void TransmitterWorker::taskTransmitData()
{
    qInfo() << "TransmitterWorker::taskTransmitData() is runnig on "<< QThread::currentThread();
    while(1){
        std::this_thread::sleep_for(std::chrono::microseconds(1));
        QMutexLocker<QMutex> mutexlocker(&GeneralData::getInstance()->mutex);
        for (auto& x: transmitter::getInstance(chanell)->getActions() )
        {
            if (!x.bIfApplied){
                std::this_thread::sleep_for(std::chrono::microseconds(1));
                emit sendData(x);
                qInfo() << x.chanel<<"\t"<<x.instruction<<"\t"<<x.arincData<< "\t" << x.bIfApplied;
            }
        }
    }
}

