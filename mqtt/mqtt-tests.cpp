/***
 * Copyright 2020 The Katla Authors
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

#include "katla/core/core.h"
#include "katla/core/default-logger.h"
#include "mqtt-errors.h"
#include "mqtt.h"

#include "gtest/gtest.h"

#include <chrono>
#include <filesystem>

namespace katla {

using namespace std::chrono_literals;

std::string helloWorld = "Hello World!";

TEST(KatlaMqttTests, CreateTest)
{
    DefaultLogger logger;

    {
        katla::Mqtt mqtt(logger);
        mqtt.init();
    }

    SUCCEED();
}

TEST(KatlaMqttTests, CreateClientTest)
{
    DefaultLogger logger;

    {
        katla::Mqtt mqtt(logger);
        auto initResult = mqtt.init();
        ASSERT_TRUE(initResult) << initResult.error().message();

        auto clientResult = mqtt.createClient("testClient");
        ASSERT_TRUE(clientResult) << clientResult.error().message();

        auto client = std::move(clientResult.value());
    }

    SUCCEED();
}

TEST(KatlaMqttTests, ConnectClientTest)
{
    DefaultLogger logger;

    {
        katla::Mqtt mqtt(logger);
        auto initResult = mqtt.init();
        ASSERT_TRUE(initResult) << initResult.error().message();

        auto clientResult = mqtt.createClient("testClient");
        ASSERT_TRUE(clientResult) << clientResult.error().message();

        auto client = std::move(clientResult.value());

        bool done = false;
        client->onConnect([&done]() {
            done = true;
        });

        std::string host = "localhost";

        auto connectResult = client->connect(host, 1883, 60);
        ASSERT_TRUE(connectResult) << katla::format("{}: {}", connectResult.error().message(), connectResult.error().description());

        std::function<std::chrono::milliseconds()> getTimestamp = []() {
          return std::chrono::duration_cast<std::chrono::milliseconds>(
              std::chrono::system_clock::now().time_since_epoch());
        };

        auto startTime = getTimestamp();

        while (!done) {
            auto timeoutElapsed = (getTimestamp() - startTime) > std::chrono::milliseconds(4000);
            ASSERT_EQ(timeoutElapsed, false); //timeout
        }
    }

    SUCCEED();
}


TEST(KatlaMqttTests, PublishClientTest)
{
    DefaultLogger logger;

    {
        katla::Mqtt mqtt(logger);
        auto initResult = mqtt.init();
        ASSERT_TRUE(initResult) << initResult.error().message();

        auto clientResult = mqtt.createClient("testClient");
        ASSERT_TRUE(clientResult) << clientResult.error().message();

        auto client = std::move(clientResult.value());

        bool done = false;
        client->onConnect([&done, &client]() {

            std::string hello = "hello world!";
            gsl::span<std::byte> span (reinterpret_cast<std::byte*>(hello.data()), hello.size());
            client->publish("hello", span, MqttQos::AtLeastOnce, false);

          done = true;
        });

        std::string host = "localhost";

        auto connectResult = client->connect(host, 1883, 60);
        ASSERT_TRUE(connectResult) << katla::format("{}: {}", connectResult.error().message(), connectResult.error().description());

        std::function<std::chrono::milliseconds()> getTimestamp = []() {
          return std::chrono::duration_cast<std::chrono::milliseconds>(
              std::chrono::system_clock::now().time_since_epoch());
        };

        auto startTime = getTimestamp();

        while (!done) {
            auto timeoutElapsed = (getTimestamp() - startTime) > std::chrono::milliseconds(4000);
            ASSERT_EQ(timeoutElapsed, false); //timeout
        }
    }

    SUCCEED();
}

} // namespace katla
