#include "ReceiverWorker.h"

#include "ArincData.h"
#include "Label.h"
#include "Equipment.h"
#include "arinc_functions.h"
#include "baseitem.h"
#include "DEI1016.h"
#include "DEI1016RasberryConfigurations.h"
#include <memory>
#include <QDebug>

ReceiverWorker::ReceiverWorker(str_t equipment, uint8_t ch):channel(ch)
{
    if (ch >= CHANNEL_DEI_MAPPING.size()) {
        qWarning() << "Invalid channel:" << ch << ", defaulting to 0";
        channel = 0;
    }
    auto mapping = CHANNEL_DEI_MAPPING[channel];
    dei = mapping.first;
    dei_channel = mapping.second;
    
    arinc_data = std::make_unique<DArincData>();
    equipments.push_back(new Equipment(equipment, EquipmentRole::Receiver));
    
    main_thread = new QThread();
    main_thread->setObjectName("Main receiver thread...");

    idle_label_cleaner_timer = new Timer();
    idle_label_cleaner_timer->setTimeout(CLEANING_CHECK_TIME);

    data_rate_timer = new Timer();
    data_rate_timer->setTimeout(DATA_RATE_EVAL_TIME);

    connect(idle_label_cleaner_timer, &Timer::onTimeout, this, &ReceiverWorker::idleLabelCleaner, Qt::DirectConnection);
    connect(data_rate_timer, &Timer::onTimeout, this, &ReceiverWorker::evalDataRates, Qt::DirectConnection);
    connect(DEI1016::getInstance(), &DEI1016::update, this, &ReceiverWorker::update, Qt::BlockingQueuedConnection);

    connect(main_thread, &QThread::started, this, &ReceiverWorker::receiveTask);
    idle_label_cleaner_timer->moveToThread(idle_cleaner_thread);
    data_rate_timer->moveToThread(data_rate_thread);
    this->moveToThread(main_thread);
}

ReceiverWorker::~ReceiverWorker()
{
    cleanup();
}

void ReceiverWorker::cleanup()
{
    stop();
    if (main_thread) {
        main_thread->quit();
        main_thread->wait();
        delete main_thread;
    }
    if (idle_cleaner_thread) {
        idle_cleaner_thread->quit();
        idle_cleaner_thread->wait();
        delete idle_cleaner_thread;
    }
    if (data_rate_thread) {
        data_rate_thread->quit();
        data_rate_thread->wait();
        delete data_rate_thread;
    }
    delete data_rate_timer;
    delete idle_label_cleaner_timer;
    for (auto* item : equipments) {
        delete item;
    }
}

void ReceiverWorker::startTasks()
{
    if (!DEI1016::getInstance()->bIfSerialOpen){
        DEI1016::getInstance()->openSerialPort();
    }
    idle_cleaner_thread->start();
    data_rate_thread->start();
    main_thread->start();
}

void ReceiverWorker::stop()
{
    m_running = false;
    if (main_thread && main_thread->isRunning()) {
        main_thread->quit();
        main_thread->wait();
    }
}

void ReceiverWorker::idleLabelCleaner()
{
    if_to_clean.store(true);
}

void ReceiverWorker::evalDataRates()
{
     Receiver::getInstance(channel)->EvalDataRates(DATA_RATE_EVAL_TIME);
}

DArincData& ReceiverWorker::getArincData()
{
    return *arinc_data;
}

Equipment* ReceiverWorker::getEquipment()
{
    return static_cast<Equipment*>(equipments[0]);
}

#include <thread>
#include <chrono>
void ReceiverWorker::receiveTask()
{
    while(m_running.load())
    {
        std::this_thread::sleep_for(std::chrono::microseconds(10));
        if (if_data_updated.load())
        {
            QMutexLocker locker(&data_mutex);
            str_t label_id = getArincData().template Get<LabelIdOctal>().toString();
            value_t value = getArincData().template Get<DataBits>();
            auto* receiver = Receiver::getInstance(channel);
             if (receiver) {
                 receiver->SetLabelData(label_id, rate.load(), value);
             }
             if_data_handled.store(true);
            if_data_updated.store(false);
        }

        if (if_to_clean.load())
        {
            auto* receiver = Receiver::getInstance(channel);
            if (receiver) {
                receiver->idleLabelCleaner();
            }
            if_to_clean.store(false);
        }
    }
}

void ReceiverWorker::update(uint8_t& dei_id, uint8_t& channel_id, float& _rate, std::bitset<ARINC32_SIZE>& arinc_bits_data)
{
    if (dei_id==dei && channel_id==dei_channel) {
        QMutexLocker locker(&data_mutex);
        arinc_data->UpdateData(arinc_bits_data);
        rate.store(_rate);
        if_data_updated.store(true);
    }
}

