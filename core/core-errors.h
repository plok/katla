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

#ifndef KATLA_CORE_ERRORS_H
#define KATLA_CORE_ERRORS_H

#include <system_error>
#include <string>

namespace katla {

    enum class CoreErrorCode {
        InitFailed,
        NotInitialized,
        AlreadyInitialized,
        CloseFailed,
        StartFailed,
        StopFailed,
        OperationFailed,
        InvalidSignal
    };

    // Define a custom error code category derived from std::error_category
    class CoreErrorCategory : public std::error_category
    {
    public:
        // Return a short descriptive name for the category
        virtual const char *name() const noexcept override final { return "basic error"; }
        // Return what each enum means in text
        virtual std::string message(int c) const override final;

        virtual std::error_condition default_error_condition(int c) const noexcept override final;
    };

    const CoreErrorCategory& coreErrorCategory();

    inline std::error_code make_error_code(katla::CoreErrorCode e) {
        return {static_cast<int>(e), katla::coreErrorCategory()};
    }

    inline std::error_code lastSystemError() {
        return std::make_error_code(static_cast<std::errc>(errno));
    }
}

#endif //KATLA_CORE_UV_ERRORS_H
