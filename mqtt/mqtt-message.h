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
#ifndef KATLA_MQTT_MESSAGE_H
#define KATLA_MQTT_MESSAGE_H

#include "outcome/outcome.hpp"

#include <gsl/span>

namespace katla {

enum class MqttQos {AtMostOnce = 0, AtLeastOnce = 1, ExactlyOnce = 2};

struct MqttMessage
{
    // Copying a MqttMessage object is dangerous, because payload holds a pointer
    // to non-owned memory
    MqttMessage() = default;
    MqttMessage(MqttMessage const&) = delete;
    MqttMessage& operator=(MqttMessage const&) = delete;

    int messageId {};
    std::string topic;
    gsl::span<std::byte> payload;
    MqttQos qos;
    bool retain {false};
};

} // namespace katla

#endif
