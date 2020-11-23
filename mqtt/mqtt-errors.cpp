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
#include "mqtt-errors.h"

std::string katla::MqttErrorCategory::message(int c) const {
    switch (static_cast<MqttErrorCodes>(c))
    {
        case MqttErrorCodes::MosquittoError:
            return "Mosquitto error";
    }

    return "unknown";
}

std::error_condition katla::MqttErrorCategory::default_error_condition(int c) const noexcept {
    return std::error_condition(c, *this);
}

const katla::MqttErrorCategory &katla::mqttErrorCategory() {
    static katla::MqttErrorCategory c;
    return c;
}


