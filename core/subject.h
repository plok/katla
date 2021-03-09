/***
 * Copyright 2019 The Katla Authors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef SUBJECT_H
#define SUBJECT_H

#include "observable.h"
#include "observer.h"

#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace katla {

template <class T> class Subject : public Observable<T>, public Observer<T> {
  public:
    virtual ~Subject() = default;

    void next(const T& value)
    {
        for (auto o : m_observers) {
            o->next(value);
        }
    }

    std::unique_ptr<Subscription> subscribe(const std::shared_ptr<Observer<T>>& observer)
    {
        m_observers.push_back(observer);

        return std::unique_ptr<Subscription>(
            new FuncSubscription([this, observer]() { this->unsubscribe(observer); }));
    }

    void unsubscribe(const std::shared_ptr<Observer<T>>& observer)
    {
        auto it = std::find(m_observers.begin(), m_observers.end(), observer);
        if (it != m_observers.end()) {
            m_observers.erase(it);
        }
    }

    void clear() { m_observers.clear(); }

  private:
    std::vector<std::shared_ptr<Observer<T>>> m_observers;
};

template<>
class Subject<void> : public Observable<void>, public Observer<void> {
  public:
    ~Subject() override = default;

    void next() override
    {
        for (const auto& o : m_observers) {
            o->next();
        }
    }

    std::unique_ptr<Subscription> subscribe(const std::shared_ptr<Observer<void>>& observer) override
    {
        m_observers.push_back(observer);

        return std::unique_ptr<Subscription>(
            new FuncSubscription([this, observer]() { 
                this->unsubscribe(observer);
        }));
    }

    void unsubscribe(const std::shared_ptr<Observer<void>>& observer)
    {
        auto it = std::find(m_observers.begin(), m_observers.end(), observer);
        if (it != m_observers.end()) {
            m_observers.erase(it);
        }
    }
    void clear() { m_observers.clear(); }

  private:
    std::vector<std::shared_ptr<Observer<void>>> m_observers;
};


}

#endif