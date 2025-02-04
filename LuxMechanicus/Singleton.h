#pragma once

template <typename T>
class Singleton {
public:
    static T& GetInstance() {
        static T instance; 
        return instance;
    }

protected:
    T& instance;
    Singleton() {}

    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;
};

