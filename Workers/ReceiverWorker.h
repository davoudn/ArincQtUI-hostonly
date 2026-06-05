#ifndef RECEIVERWORKER_H
#define RECEIVERWORKER_H
#include "types.h"
#include "Timer.h"
#include "PointerVector.h"
#include "Receiver.h"
#include "TreeDataModel.h"

#include <QThread>
#include <QtConcurrent/QtConcurrent>
#include <memory>
#include "singleton.h"
template <typename T, int I>
class DeviceDelegate;

class DArincData;
class Equipment;
class BaseItem;
class transmitter;
class Receiver;

#define DEVICEONE_CHANELLONE 11
#define DEVICEONE_CHANELLTWO 12

class ReceiverWorker: public QObject
, public SingletonArray<ReceiverWorker,4>
{
    Q_OBJECT
    ReceiverWorker(str_t equipment, uint8_t ch);
    ReceiverWorker( ) = delete;
public:
    ~ReceiverWorker();
    DArincData& getArincData();
    PointerVector<BaseItem>& getEquipments();
    Equipment* getEquipment();
    void startTasks();
    void stop();
public:
    std::atomic<bool> if_data_updated{false};
    std::atomic<bool> if_data_handled{true};
    std::atomic<bool> if_to_clean{false};
    Receiver *parent=nullptr;
    std::unique_ptr<DArincData> arinc_data;
    std::atomic<float> rate{0.f};
    uint8_t dei = 0;
    uint8_t dei_channel = 0;
    QMutex data_mutex;
public slots:
    void idleLabelCleaner();
    void evalDataRates();
    void receiveTask();
    void update(uint8_t& dei, uint8_t& channel, float& _rate
        , std::bitset<ARINC32_SIZE>& arinc_bits_data);
protected:
    void cleanup();
protected:
    PointerVector<BaseItem> equipments;
    str_t default_equipment;
    Timer* idle_label_cleaner_timer = nullptr;
    Timer* data_rate_timer = nullptr;
    bool if_enabled = false;
    QThread* main_thread = nullptr;
    QThread* idle_cleaner_thread = nullptr;
    QThread* data_rate_thread = nullptr;
    uint8_t channel = 0;
    std::atomic<bool> m_running{true};
};
#endif // RECEIVERWORKER_H



