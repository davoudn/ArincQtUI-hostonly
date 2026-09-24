#pragma once

#include <array>
#include <memory>
#include <exception>

template<typename T, int N>
class SingletonArray {
protected:
    SingletonArray() = default;
public:
    static T* getInstance(int index) {
        static std::array<std::unique_ptr<T>, N> instances{};
        if (index < 0 || index >= N) {
            throw std::runtime_error(" Out of index access to instances ...");
        }
        if (!instances[index]) {
            instances[index] = std::make_unique<T>(nullptr, static_cast<int>(index));
        }
        return instances[index].get();
    }
};

template<typename T>
class Singleton{
protected:
    Singleton() = default;
public:
    static T* getInstance(int index) {
        static std::unique_ptr<T> instance;
        if (!instance) {
            instance = std::make_unique<T>(nullptr);
        }
        return instance.get();
    }
};