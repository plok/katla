/***
 * Copyright 2022 The Katla Authors
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
#include "core-uv/uv-posix-process.h"
#include "googletest/googletest/include/gtest/gtest.h"
#include "core-uv/uv-core-application.h"

#include "gtest/gtest.h"

namespace katla {

    using namespace std::chrono_literals;
    
    TEST(KatlaUvTests, PosixChildStatusTest) {

        katla::Stopwatch stopwatch;
        stopwatch.start();

        katla::UvPosixProcess process;
        auto spawnResult = process.spawn("/usr/bin/env", {"sleep", "4"}, "./", {});
        ASSERT_FALSE(spawnResult.has_error());
        
        std::this_thread::sleep_for(100ms);

        auto statusResult = process.status();
        
        ASSERT_TRUE(statusResult.has_value() && process.status().value() == UvPosixProcess::Status::Running);

        std::this_thread::sleep_for(4s);

        statusResult = process.status();
        ASSERT_TRUE(statusResult.has_value() && process.status().value() == UvPosixProcess::Status::Exitted);
    }

    TEST(KatlaUvTests, PosixChildOnCloseTest) {

        katla::Stopwatch stopwatch;
        stopwatch.start();

        auto& coreApp = UvCoreApplication::instance();
        [[maybe_unused]] auto _ = coreApp.init();

        bool closed = false;

        katla::UvPosixProcess processA;
        katla::UvPosixProcess processB;
        processA.onClose([&closed]() {
            katla::printInfo("ProcessClosed!");
            closed = true;
        });
        auto spawnAResult = processA.spawn("/usr/bin/env", {"sleep", "8"}, "./", {});
        auto spawnBResult = processB.spawn("/usr/bin/env", {"sleep", "4"}, "./", {}); // B will close earlier than A
        ASSERT_FALSE(spawnAResult.has_error());
        ASSERT_FALSE(spawnBResult.has_error());
        
        auto timerResult = coreApp.createTimer();
        ASSERT_FALSE(timerResult.has_error());
        auto timer = std::move(timerResult.value());
        _ = timer->start(100ms, [&]() {
            auto msec = stopwatch.msecsElapsed();
            if (msec < 3000) {
                ASSERT_TRUE(processA.status().value() == UvPosixProcess::Status::Running);
                ASSERT_TRUE(processB.status().value() == UvPosixProcess::Status::Running);
            }

            if (msec > 5000 && msec < 7000) {
                ASSERT_TRUE(processA.status().value() == UvPosixProcess::Status::Running);
                ASSERT_TRUE(processB.status().value() == UvPosixProcess::Status::Exitted);
                ASSERT_FALSE(closed);
            }

            if (msec > 9000) {
                ASSERT_TRUE(processA.status().value() == UvPosixProcess::Status::Exitted);
                ASSERT_TRUE(processB.status().value() == UvPosixProcess::Status::Exitted);
                ASSERT_TRUE(closed);

                _ = timer->stop();
                _ = coreApp.stop();
            }           
        });

        _ = coreApp.run();

        _ = timer->close();
        _ = coreApp.close();

        SUCCEED();
    }
}

