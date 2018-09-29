#ifndef OBSERVER_H
#define OBSERVER_H

#include <string>
#include <memory>
#include <optional>
#include <functional>

template <class T>
class Observer
{
public:
    virtual void publish(T value) = 0;
};

template <class T>
class FuncObserver : public Observer<T>
{
public:
    FuncObserver(std::function<void(T)> func) {
        _func = func;
    }

    void publish(T value) {
        if (_func) {
            _func(value);
        }
    }

private:
    std::function<void(T)> _func;
};

#endif