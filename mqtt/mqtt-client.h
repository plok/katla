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
#ifndef KATLA_MQTT_CLIENT_H
#define KATLA_MQTT_CLIENT_H

#include "katla/core/logger.h"
#include "katla/core/subject.h"
#include "katla/core/thread.h"
#include "katla/core/worker-thread.h"

#include "katla/mqtt/mqtt-message.h"

#include "mosquitto.h"
#include "outcome/outcome.hpp"
#include <map>

namespace katla {

class Mqtt;

class MqttClient {
    friend Mqtt; // TODO separate interface instead

// TODO message timeouts
// TODO message ack / nack
// TODO unsubscribe

  public:
    explicit MqttClient(Logger& logger);
    ~MqttClient();

    outcome::result<void, Error> connect(const std::string& host, int port, int keepAliveSeconds);
    outcome::result<void, Error> disconnect();

    outcome::result<void, Error> publish(std::string topic, gsl::span<std::byte> payload, MqttQos qos, bool retain);
    outcome::result<std::unique_ptr<katla::Subscription>, Error> subscribe(std::string subPattern, MqttQos qos, const std::function<void(const MqttMessage& message)>& callback);

    std::unique_ptr<katla::Subscription> onConnect(const std::function<void(void)>& callback) {
        return m_onConnectSubject.subscribe(std::make_shared<katla::FuncObserver<void>>(callback));
    }
    std::unique_ptr<katla::Subscription> onDisconnect(const std::function<void(void)>& callback) {
        return m_onDisconnectSubject.subscribe(std::make_shared<katla::FuncObserver<void>>(callback));
    }
    std::unique_ptr<katla::Subscription> onSubscribe(const std::function<void(const std::string&)>& callback) {
        return m_onSubscribeSubject.subscribe(std::make_shared<katla::FuncObserver<std::string>>(callback));
    }

  private:
    outcome::result<void, Error> init(const std::string& clientName);

    void handleConnect(int rc);
    void handleDisconnect(int rc);
    void handleLogMessage(int level, const char* message);
    void handleMessage(const struct mosquitto_message* message);
    void handleSubscribe(int mid, int qos_count, int granted_qos);

    mosquitto* m_client {};
    Logger& m_logger;

    bool m_connected {};
    katla::WorkerThread m_workerThread;

    katla::Subject<void> m_onConnectSubject;
    katla::Subject<void> m_onDisconnectSubject;
    katla::Subject<std::string> m_onSubscribeSubject;
    std::map<std::string, katla::Subject<MqttMessage> > m_onMessageSubject;
    std::map<int, std::string> m_subscriptions;
};

} // namespace syncer

#endif
