#include "TransmitterWorker.h"
#include "Label.h"
#include "Equipment.h"
#include "arinc_functions.h"
#include "Transmitter.h"
#include "Timer.h"
#include "DEI1016.h"
#include <thread>
#include <chrono>
#include "types.h"
#include "DEI1016RasberryConfigurations.h"
#include "generaldata.h"
#include "action.h"
#include <memory>
#include <QDebug>

TransmitterWorker::TransmitterWorker(str_t equipment, int ch)
{
    channel = ch;
    if (ch >= static_cast<int>(CHANNEL_DEI_MAPPING.size())) {
        qWarning() << "Invalid channel:" << ch << ", defaulting to 0";
        channel = 0;
    }
    auto mapping = CHANNEL_DEI_MAPPING[channel];
    default_equipment = "002";
    equipments.push_back(new Equipment(default_equipment,EquipmentRole::Transmitter));
    arinc_data = std::make_unique<DArincData>();
    
    main_thread = new QThread();
    data_rate_thread = new QThread();
    main_thread->setObjectName(" Main Transmitter thread... ");
    data_rate_thread->setObjectName(" Transmitter data rate thread... ");
    data_rate_timer = new Timer(this);
    data_rate_timer->setTimeout(ACTION_CLEANER_TIME);
    
    connect(main_thread, &QThread::started, this, &TransmitterWorker::taskTransmitData);
    connect(data_rate_thread, &QThread::started, data_rate_timer, &Timer::counterTask);
    connect(data_rate_timer, &Timer::onTimeout, this, &TransmitterWorker::actionListCleaner, Qt::DirectConnection);
    connect(this, &TransmitterWorker::sendData, DEI1016::getInstance(), &DEI1016::sendData, Qt::BlockingQueuedConnection);
    
    data_rate_timer->moveToThread(data_rate_thread);
    this->moveToThread(main_thread);
}

TransmitterWorker::~TransmitterWorker()
{
    cleanup();
}

void TransmitterWorker::cleanup()
{
    stop();
    if (main_thread) {
        main_thread->quit();
        main_thread->wait();
        delete main_thread;
    }
    if (data_rate_thread) {
        data_rate_thread->quit();
        data_rate_thread->wait();
        delete data_rate_thread;
    }
    delete data_rate_timer;
    for (auto* item : equipments) {
        delete item;
    }
}

void TransmitterWorker::startTasks()
{
    DEI1016::getInstance();
    if (!DEI1016::getInstance()->bIfSerialOpen){
        DEI1016::getInstance()->openSerialPort();
    }
    data_rate_thread->start();
    main_thread->start();
}

void TransmitterWorker::stop()
{
    m_running = false;
    if (main_thread && main_thread->isRunning()) {
        main_thread->quit();
        main_thread->wait();
    }
}

void TransmitterWorker::actionListCleaner()
{
    QMutexLocker locker(&GeneralData::getInstance()->mutex);
    for (auto it = GeneralData::getInstance()->getActions().begin();  it!=GeneralData::getInstance()->getActions().end(); it++){
        if ( (*it)->if_applied ){
        }
    }
}

void TransmitterWorker::run()
{
    taskTransmitData();
}

void TransmitterWorker::incrementLabelsDataRateCounter()
{
}

TransmitterWorker* TransmitterWorker::getInstance(int ch)
{
    if (ch >= static_cast<int>(CHANNEL_DEI_MAPPING.size())) {
        qWarning() << "Invalid channel:" << ch << ", defaulting to 0";
        ch = 0;
    }
    static std::array<std::unique_ptr<TransmitterWorker>, 2> instances{};
    if (!instances[ch]) {
        instances[ch] = std::make_unique<TransmitterWorker>("002", ch);
    }
    return instances[ch].get();
}

Equipment* TransmitterWorker::getEquipment()
{
    return static_cast<Equipment*>(equipments[0]);
}

void TransmitterWorker::taskTransmitData()
{
    while(m_running.load())
    {
        std::this_thread::sleep_for(std::chrono::microseconds(10));
        QMutexLocker mutex_locker(&GeneralData::getInstance()->mutex);
        for (int i=0; i < GeneralData::getInstance()->getActions().size(); i++)
        {
            if (!GeneralData::getInstance()->getActions()[i]->if_applied){
                emit sendData(GeneralData::getInstance()->getActions()[i]->toPacket());
                GeneralData::getInstance()->getActions()[i]->if_applied = true;
            }
        }
    }
}

