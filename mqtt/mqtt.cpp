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
#include "mqtt.h"

#include "katla/core/core.h"
#include "katla/mqtt/mqtt-errors.h"

#include "mosquitto.h"

namespace katla {

Mqtt::Mqtt(Logger& logger) :
    m_logger(logger)
{
}

Mqtt::~Mqtt() {
    int result = mosquitto_lib_cleanup();
    if (result != MOSQ_ERR_SUCCESS) {
        m_logger.error(katla::format("Error cleanup libmosquitto: {}", result));
    }
}

katla::result<void, Error>  Mqtt::init() {
    int result = mosquitto_lib_init();
    if (result != MOSQ_ERR_SUCCESS) {
        return Error(make_error_code(MqttErrorCodes::MosquittoError), katla::format("Error initializing libmosquitto: {}"));
    }

    int major, minor, revision = {};
    mosquitto_lib_version(&major, &minor, &revision);

    m_logger.debug(katla::format("Loaded mosquitto library version: {}.{}.{}", major, minor, revision));

    return outcome::success();
}

katla::result<std::unique_ptr<MqttClient>, Error> Mqtt::createClient(std::string name) { // NOLINT(readability-convert-member-functions-to-static)
    auto mqttClient = std::make_unique<MqttClient>(m_logger);

    auto initResult = mqttClient->init(name);
    if (!initResult) {
        return initResult.error();
    }

    return mqttClient;
}

} // namespace syncer