#include "transmitterworker.h"
#include "Label.h"
#include "Equipment.h"
#include "arinc_functions.h"
#include "transmitter.h"
#include "Timer.h"
#include "generaldata.h"

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

void TransmitterWorker::startTasks(){
    mainThread->start();
    dataRateThread->start();
}
/*
 * SLOTS
 */

void TransmitterWorker::actionListCleaner(){
    QMutexLocker<QMutex> locker(&GeneralData::getInstance()->mutex);
    for (auto it = transmitter::getInstance()->getActions().begin();  it!=transmitter::getInstance()->getActions().end(); it++){
        if ( (*it).bIfApplied ){
            transmitter::getInstance()->getActions().erase(it);
        }
    }
}


void TransmitterWorker::run(){
    taskTransmitData();
}

void TransmitterWorker::incrementLabelsDataRateCounter(){
     transmitter::getInstance()->incrementLabelsDataRateCounter();
   // qInfo() << "TransmitterWorker::incrementLabelsDataRateCounter() runs on " << QThread::currentThread();
}

void TransmitterWorker::taskTransmitData(){
    qInfo() << "TransmitterWorker::taskTransmitData() is runnig on "<< QThread::currentThread();
    while(1){
        QMutexLocker<QMutex> mutexlocker(&GeneralData::getInstance()->mutex);
        auto dataToSend = transmitter::getInstance()->getActions();
            for (auto& x: dataToSend ){
//#ifdef RASBERRY

//#endif
            }
        }
}

