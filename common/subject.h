#ifndef SUBJECT_H
#define SUBJECT_H

#include "observer.h"
#include "observable.h"

#include <string>
#include <memory>
#include <optional>
#include <vector>

template<class T>
class Subject : public Observable<T>, public Observer<T>
{
public:
    void publish(T value)
    {
        for(auto o : _observers) {
            o->publish(value);
        }
    }
    
    void subscribe(std::shared_ptr<Observer<T>> observer)
    {
        _observers.push_back(observer);
    }

    void unsubscribe(std::shared_ptr<Observer<T>> observer);    
private:
    std::vector<std::shared_ptr<Observer<T>>> _observers;
};

#endif