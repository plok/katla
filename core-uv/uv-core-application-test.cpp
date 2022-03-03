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

#include "core/core.h"
#include "core/stopwatch.h"
#include "uv-core-application.h"

#include "gtest/gtest.h"

#include <gsl/span>

#include <fmt/format.h>
#include <variant>
#include <chrono>

#include <utility>
#include <memory>
#include <functional>
#include <thread>

#include <cstdlib>
#include <unistd.h>
#include <exception>
#include <fmt/format.h>
#include <variant>
#include <chrono>

namespace katla {

    using namespace std::chrono_literals;

    std::string helloWorld = "Hello World!";

    
/***
 * - parent starts listening
 * - child sends message
 * - parent receives message
 */
    TEST(KatlaUvTests, ProperCloseTest) {

        UvCoreApplication coreApp;
        coreApp.init();
        auto timerResult = coreApp.createTimer();
        ASSERT_TRUE(timerResult) << timerResult.error().message();

        auto timer = std::move(timerResult.value());

        coreApp.onClose([&coreApp, &timer]() {
            timer->stop();
            coreApp.stop();
        });

        timer->start(1s, [&timer, &coreApp]() {
            katla::printInfo(helloWorld);

            timer->stop();
            coreApp.stop();
        });

        coreApp.run();

        katla::printInfo("exit of event loop");

        timer->close();
        coreApp.close();

        katla::printInfo("end");

        GTEST_SUCCEED();
    }

    outcome::result<std::string> createTemporaryDir() {
        std::string dirTemplate = "/tmp/katla-test-XXXXXX";
        if (mkdtemp(dirTemplate.data()) == nullptr) {
            return std::make_error_code(static_cast<std::errc>(errno));
        }
        return dirTemplate;
    }
}

