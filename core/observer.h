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

#ifndef OBSERVER_H
#define OBSERVER_H

#include <string>
#include <memory>
#include <optional>
#include <functional>
#include <utility>

namespace katla {

template <class T> class Observer {
  public:
    virtual ~Observer() = default;
    virtual void next(const T& value) = 0;
};

template <>
class Observer<void> {
  public:
    virtual ~Observer() = default;
    virtual void next() = 0;
};

template <class T> class FuncObserver : public Observer<T> {
  public:
    explicit FuncObserver(std::function<void(const T&)> func) { _func = func; }

    void next(const T& value)
    {
        if (_func) {
            _func(value);
        }
    }

  private:
    std::function<void(const T&)> _func;
};

template <>
class FuncObserver<void> : public Observer<void> {
  public:
    explicit FuncObserver(std::function<void(void)> func) { _func = std::move(func); }
    ~FuncObserver() override = default;

    void next() override
    {
        if (_func) {
            _func();
        }
    }

  private:
    std::function<void(void)> _func;
};



}

#endif