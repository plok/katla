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

#include <algorithm>
#include <memory>
#include <mutex>
#include <optional>
#include <string>
#include <vector>

namespace katla {

template <class T> class Subject : public Observable<T>, public Observer<T> {
  public:
    Subject() = default;
    Subject(Subject&) = delete;
    virtual ~Subject() = default;

    void next(const T& value)
    {
        std::vector<std::shared_ptr<Observer<T>>> observers;
        {
            std::scoped_lock lock(m_mutex);
            observers = m_observers;
        }

        for (auto o : observers) {
            o->next(value);
        }
    }

    std::unique_ptr<Subscription> subscribe(const std::shared_ptr<Observer<T>>& observer)
    {
        std::scoped_lock lock(m_mutex);
        m_observers.push_back(observer);

        return std::unique_ptr<Subscription>(
            new FuncSubscription([this, observer]() { this->unsubscribe(observer); }));
    }

    void unsubscribe(const std::shared_ptr<Observer<T>>& observer)
    {
        std::scoped_lock lock(m_mutex);
        auto it = std::find(m_observers.begin(), m_observers.end(), observer);
        if (it != m_observers.end()) {
            m_observers.erase(it);
        }
    }

    void clear() {
        std::scoped_lock lock(m_mutex);
        m_observers.clear();
    }

  private:
    std::mutex m_mutex;
    std::vector<std::shared_ptr<Observer<T>>> m_observers;
};

template<>
class Subject<void> : public Observable<void>, public Observer<void> {
  public:
    Subject() = default;
    Subject(Subject&) = delete;
    ~Subject() override = default;

    void next() override
    {
        std::vector<std::shared_ptr<Observer<void>>> observers;
        {
            std::scoped_lock lock(m_mutex);
            observers = m_observers;
        }

        for (const auto& o : observers) {
            o->next();
        }
    }

    std::unique_ptr<Subscription> subscribe(const std::shared_ptr<Observer<void>>& observer) override
    {
        std::scoped_lock lock(m_mutex);
        m_observers.push_back(observer);

        return std::unique_ptr<Subscription>(
            new FuncSubscription([this, observer]() { 
                this->unsubscribe(observer);
        }));
    }

    void unsubscribe(const std::shared_ptr<Observer<void>>& observer)
    {
        std::scoped_lock lock(m_mutex);

        auto it = std::find(m_observers.begin(), m_observers.end(), observer);
        if (it != m_observers.end()) {
            m_observers.erase(it);
        }
    }
    void clear() {
        std::scoped_lock lock(m_mutex);
        m_observers.clear();
    }

  private:
    std::mutex m_mutex;
    std::vector<std::shared_ptr<Observer<void>>> m_observers;
};


}

#endif