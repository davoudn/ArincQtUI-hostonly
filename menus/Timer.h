#pragma once
#include <map>
#include <functional>
#include <QThread>
#include <QtConcurrent/QtConcurrent>

class Single {};
class Multi{};


class Equipment;
class BaseDevice;
class TransmitterWorker;

class Timer: public QObject {
    Q_OBJECT
public:
    explicit Timer(QObject* parent = nullptr);
    ~Timer();

    void setTimeout(int t);
    void  stop();
    void  reset();
    float getTime();
    virtual void run() ;
public slots:
    void  counterTask();

signals:
    void onTimeout();

protected:
    uint32_t timeOut = 0.f;
    uint32_t counter = 0;
    bool bIfStartTask = true;
    TransmitterWorker* transmitterWorker = nullptr;
};


