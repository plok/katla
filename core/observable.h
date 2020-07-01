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

#ifndef OBSERVABLE_H
#define OBSERVABLE_H

#include "observer.h"

#include <memory>
#include <optional>
#include <string>
#include <utility>

namespace katla {

class Subscription {
  public:
    virtual ~Subscription() = default;
    virtual void unsubscribe() = 0;
};

class FuncSubscription : public Subscription {
  public:
    explicit FuncSubscription(std::function<void(void)> unsubscribeCallback) : m_unsubscribe(std::move(unsubscribeCallback)) {}
    ~FuncSubscription() override = default;

    void unsubscribe() override { m_unsubscribe(); }

  private:
    std::function<void(void)> m_unsubscribe;
};

template <class T> class Observable {
  public:
    virtual ~Observable() = default;
    virtual std::unique_ptr<Subscription> subscribe(std::shared_ptr<Observer<T>> observer) = 0;
};

} // namespace katla

#endif