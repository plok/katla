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
#ifndef KATLA_MQTT_H
#define KATLA_MQTT_H

#include "katla/core/logger.h"

#include "katla/mqtt/mqtt-client.h"

namespace katla {
  
class Mqtt {
  public:
    Mqtt(Logger& logger);
    ~Mqtt();

    katla::result<void, Error> init();
    katla::result<std::unique_ptr<MqttClient>, Error> createClient(std::string name);
  private:
    Logger& m_logger;
};

} // namespace katla

#endif
