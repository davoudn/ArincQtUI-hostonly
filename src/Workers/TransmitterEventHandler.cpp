#include "transmitterworker.h"
#include "Label.h"
#include "Equipment.h"
#include "arinc_functions.h"
#include "transmitter.h"
#include "Timer.h"
#include "DEI1016.h"
#include <thread>
#include <chrono>
#include "Types.h"
#include "DEI1016RasberryConfigurations.h"
#include "Generaldata.h"
#include "Event.h"
#include "Bitutils.h"

Equipment*  TransmitterWorker:: getEquipment()
{
    return static_cast<Equipment*>(equipments[0]);
}

PointerVector<BaseItem>& TransmitterWorker::getEquipments()
{
    return equipments;
}

TransmitterWorker::TransmitterWorker(str_t _equipment, int ch)
{
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
    connect(dataRateTimer, &Timer::onTimeout, this, &TransmitterWorker::actionListCleaner,  Qt::DirectConnection);
    connect(this, &TransmitterWorker::sendData, DEI1016::getInstance(), &DEI1016::sendData, Qt::DirectConnection);
    dataRateTimer->moveToThread(dataRateThread);
    this->moveToThread(mainThread);

}

TransmitterWorker::~TransmitterWorker()
{

}

void TransmitterWorker::startTasks()
{
    mainThread->start();
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
                for (int i=0; i < FRAME_POCKET_SIZE; i++){
                    txBuff[i] = d.second[i];
                }
                emit sendData(txBuff);
            }
        }
    }
}

