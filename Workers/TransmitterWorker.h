#ifndef TRANSMITTERWORKER_H
#define TRANSMITTERWORKER_H

#include "types.h"
#include "PointerVector.h"
#include "TreeDataModel.h"

#include <QObject>
#include <QThread>
#include <QtConcurrent/QtConcurrent>
#include <memory>
#include <atomic>

template <typename T, int I>
class DeviceDelegate;
class DArincData;
class Equipment;
class BaseItem;
class Transmitter;
class Timer;
class BaseAction;

class TransmitterWorker: public QObject {
    Q_OBJECT
    TransmitterWorker(str_t equipment, int ch);
    TransmitterWorker() = delete;

public:
    virtual ~TransmitterWorker() override;
    static TransmitterWorker* getInstance(int ch);
    virtual void run();
    inline PointerVector<BaseItem>& getEquipments(){
        return equipments;
    }
    Equipment* getEquipment();
    void startTasks();
    void stop();
    int channel = 0;
    std::atomic<bool> m_running{true};
public slots:
    void taskTransmitData();
    void incrementLabelsDataRateCounter();
    void actionListCleaner();
signals:
    bool sendData(char* ac);

protected:
    void cleanup();

    PointerVector<BaseItem> equipments;
    std::unique_ptr<DArincData> arinc_data;

    std::atomic<bool> if_data_updated{false};
    std::atomic<bool> if_data_handled{true};
    str_t default_equipment;

    QThread* main_thread = nullptr;
    QThread* data_rate_thread = nullptr;
    QMutex data_mutex;
    Timer* data_rate_timer = nullptr;
};

#endif // TRANSMITTERWORKER_H
