#ifndef OBSERVABLE_H
#define OBSERVABLE_H

#include <string>
#include <memory>
#include <optional>

#include "observer.h"

template<class T>
class Observable
{
public:
    virtual void subscribe(std::shared_ptr<Observer<T>> observer) = 0;
};

#endif