#pragma once
#include <chrono>
#include <map>
#include <thread>
#include <functional>
#include <QThread>
#include <QtConcurrent/QtConcurrent>

class Single {};
class Multi{};

template <typename T>
class Timer;

class Equipment;



template <>
class Timer<Single>: public QThread {


public:
    void SetTimeOut(int t);
    void Start();

    std::function<void(void)> CallBack;

protected:
    uint32_t TimeOut = 0.f;
    uint32_t Counter = 0;

    std::function<void(void)> Increment;
};

template <>
class Timer<Multi> {

    std::thread* Task = nullptr;

public:
    Timer(Equipment* _equipment);
    void Start();

protected:
    Equipment* equipment = nullptr;
    std::function<void(void)> Increment;
};
