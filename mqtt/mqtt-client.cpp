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
#include "mqtt-client.h"

#include "katla/core/core.h"
#include "katla/mqtt/mqtt-errors.h"

#include <memory>
#include <mutex>
#include <errno.h>

namespace katla {

MqttClient::MqttClient(Logger& logger) : m_logger(logger) {}

MqttClient::~MqttClient()
{
    if (m_client) {
        if (m_connected) {
            auto _ = disconnect();
        }

        m_mosquittoLoopStarted = false;
        mosquitto_loop_stop(m_client, true); // TODO add start / stop methods instead?
        mosquitto_destroy(m_client);
        m_client = nullptr;
    }
}

katla::result<void, Error> MqttClient::init(const std::string& clientName)
{
    if (m_client) {
        return Error(make_error_code(MqttErrorCodes::MosquittoError), katla::format("Already initialized!"));
    }

    m_client = mosquitto_new(clientName.c_str(), false, this);
    if (m_client == nullptr) {
        auto errorCode = std::make_error_code(static_cast<std::errc>(errno));
        return Error(make_error_code(MqttErrorCodes::MosquittoError),
            katla::format("Failed creating mosquitto client - {}", errorCode.message()));
    }

    mosquitto_connect_callback_set(m_client, [](struct mosquitto* /*client*/, void* obj, int rc) {
        auto mqttClient = reinterpret_cast<MqttClient*>(obj);
        mqttClient->handleConnect(rc);
    });

    mosquitto_disconnect_callback_set(m_client, [](struct mosquitto* /*client*/, void* obj, int rc) {
        auto mqttClient = reinterpret_cast<MqttClient*>(obj);
        mqttClient->handleDisconnect(rc);
    });

    mosquitto_log_callback_set(m_client, [](struct mosquitto* /*client*/, void* obj, int level, const char* message) {
        auto mqttClient = reinterpret_cast<MqttClient*>(obj);
        mqttClient->handleLogMessage(level, message);
    });

    mosquitto_message_callback_set(m_client,
                                   [](struct mosquitto* /*client*/, void* obj, const struct mosquitto_message* message) {
                                       auto mqttClient = reinterpret_cast<MqttClient*>(obj);
                                       mqttClient->handleMessage(message);
                                   });

    mosquitto_subscribe_callback_set(m_client,
                                   [](struct mosquitto* /*client*/, void* obj, int mid, int qos_count, const int* granted_qos) {
                                       auto mqttClient = reinterpret_cast<MqttClient*>(obj);
                                       mqttClient->handleSubscribe(mid, qos_count, *granted_qos);
                                   });

    return outcome::success();
}

katla::result<void, Error> MqttClient::connect(const std::string& host, int port, int keepAliveSeconds)
{
    int connectResult = mosquitto_connect_async(m_client, host.c_str(), port, keepAliveSeconds);
    if (connectResult != MOSQ_ERR_SUCCESS) {
        return makeMosquittoError(connectResult);
    }

    // The mosquitto docs state that <mosquitto_connect_async> may be called before or after <mosquitto_loop_start>,
    // but when it is called after, the following bug occurs: https://github.com/eclipse/mosquitto/issues/848.
    // So first call <mosquitto_connect_async>.
    if (!m_mosquittoLoopStarted) {
        // MqttClient::connect function is supposed to be called multiple times,
        // but the mosquitto loop should only be started once.
        int result = mosquitto_loop_start(m_client);
        if (result != MOSQ_ERR_SUCCESS) {
            return Error(make_error_code(MqttErrorCodes::MosquittoError), mosquitto_strerror(result));
        }
        m_mosquittoLoopStarted = true;
        m_logger.debug(katla::format("MQTT: Mosquitto loop started."));
    }

    return outcome::success();
}

katla::result<void, Error> MqttClient::disconnect()
{
    int result = mosquitto_disconnect(m_client);
    if (result != MOSQ_ERR_SUCCESS) {
        return makeMosquittoError(result);
    }

    return outcome::success();
}

void MqttClient::handleConnect(int rc)
{
    m_logger.debug(katla::format("Connected: {}", rc));

    switch (rc) {
    case 0: {
        m_connected = true;
        m_onConnectSubject.next();
        break; // 0 - success
    }
    case 1:
        m_logger.error("MQTT: Connection refused: unacceptable protocol version");
        break;
    case 2:
        m_logger.error("MQTT: Connection refused: identifier rejected");
        break;
    case 3:
        m_logger.error("MQTT: Connection refused: broker unavailable");
        break;
    default:
        m_logger.error("MQTT: Connection refused: unknown reason");
    }
}
void MqttClient::handleDisconnect(int rc)
{
    m_connected = false;

    if (rc) {
        m_logger.debug(katla::format("MQTT: Unexpected disconnect."));
    } else {
        m_logger.debug(katla::format("MQTT: Remote gracefully closed the connection."));
    }

    m_onDisconnectSubject.next();
}

void MqttClient::handleLogMessage(int level, const char* message)
{
    if (level == MOSQ_LOG_INFO || level == MOSQ_LOG_NOTICE) {
        m_logger.info(message);
    }
    if (level == MOSQ_LOG_WARNING || level == MOSQ_LOG_ERR) {
        m_logger.error(message);
    }
    if (level == MOSQ_LOG_DEBUG) {
        //m_logger.info(message);
    }
}

void MqttClient::handleSubscribe(int mid, int /*qos_count*/, int /*granted_qos*/)
{
   //m_logger.info(katla::format("SUBACK received; mid={} qos_count={} granted_qos={}", mid, qos_count, granted_qos));
   m_onSubscribeSubject.next(m_subscriptions[mid]);
}

void MqttClient::handleMessage(const struct mosquitto_message* mosqMessage) {
    MqttMessage message;
    message.messageId = mosqMessage->mid;
    message.topic = mosqMessage->topic;
    message.payload = gsl::span<std::byte>(reinterpret_cast<std::byte*>(mosqMessage->payload), mosqMessage->payloadlen);
    message.qos = static_cast<MqttQos>(mosqMessage->qos);
    message.retain = mosqMessage->retain;

    for (auto& subscr : m_onMessageSubject)
    {
        bool topicMatch = false;
        int result = mosquitto_topic_matches_sub(subscr.first.c_str(), mosqMessage->topic, &topicMatch);
        if (result != MOSQ_ERR_SUCCESS) {
            continue;
        }
        if (topicMatch) {
            subscr.second->next(message);
        }
    }
}

katla::result<void, Error>
MqttClient::publish(std::string topic, gsl::span<std::byte> payload, MqttQos qos, bool retain)
{
    int messageId = {};
    int result = mosquitto_publish(
        m_client, &messageId, topic.c_str(), payload.size(), payload.data(), static_cast<int>(qos), retain);
    if (result != MOSQ_ERR_SUCCESS) {
        return makeMosquittoError(result);
    }

    return outcome::success();
}

katla::result<std::unique_ptr<katla::Subscription>, Error> MqttClient::subscribe(std::string subPattern, MqttQos qos, const std::function<void(const MqttMessage& message)>& callback)
{
    int messageId = {};
    int result = mosquitto_subscribe(m_client, &messageId, subPattern.c_str(), static_cast<int>(qos));
    if (result != MOSQ_ERR_SUCCESS) {
        return makeMosquittoError(result);
    }

    m_subscriptions[messageId] = subPattern;
    
    auto it = m_onMessageSubject.find(subPattern);
    if (it == m_onMessageSubject.end()) {
        m_onMessageSubject[subPattern] = std::make_shared<Subject<MqttMessage>>();
    }

    return m_onMessageSubject[subPattern]->subscribe(std::make_shared<katla::FuncObserver<MqttMessage>>(callback));
}

Error MqttClient::makeMosquittoError(int error)
{
    if (error == MOSQ_ERR_SUCCESS) {
        return {};
    }

    if (error == MOSQ_ERR_ERRNO) {
        char errnoBuffer[ 256 ];
        char* stringPtr = strerror_r( errno, errnoBuffer, 256 );

        return Error(make_error_code(MqttErrorCodes::MosquittoError), katla::format("System call failed: {}", std::string(stringPtr)));
    }

    return Error(make_error_code(MqttErrorCodes::MosquittoError), mosquitto_strerror(error));
}

} // namespace katla