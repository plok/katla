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
#ifndef KATLA_DEFAULT_LOGGER_H_
#define KATLA_DEFAULT_LOGGER_H_

#include "katla/core/core.h"
#include "katla/core/logger.h"
#include "error.h"

#include <exception>
#include <string>

namespace katla {

class DefaultLogger : public Logger {
  public:
    DefaultLogger() = default;
    ~DefaultLogger() override = default;

    void info(const std::string_view& message) override { katla::printInfo(message); };
    void warning(const std::string_view& message) override { katla::printError(message); };
    void error(const std::string_view& message) override { katla::printError(message); };

    void error(const Error& error) override {
        katla::printError( error.message() );
    };
    void exception(const std::exception& ex) override {
        katla::printError( ex.what() );
    };
};

} // namespace katla

#endif
