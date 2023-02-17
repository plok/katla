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
#include "core-uv/uv-core-application.h"
#include "core-uv/uv-posix-process.h"

#include "gtest/gtest.h"

#include <chrono>
#include <thread>

namespace katla {

    using namespace std::chrono_literals;
    std::string helloWorld = "Hello World!";

    TEST(KatlaUvTests, ProperCloseTest) {

        auto& coreApp = UvCoreApplication::instance();
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
}

