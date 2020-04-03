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
#include "posix-errors.h"

std::string katla::PosixErrorCategory::message(int c) const {
    switch (static_cast<PosixErrorCodes>(c))
    {
        case PosixErrorCodes::InvalidDomain:
            return "Invalid protocol domain";
        case PosixErrorCodes::InvalidType:
            return "Invalid protocol type";
    }

    return "unknown";
}

std::error_condition katla::PosixErrorCategory::default_error_condition(int c) const noexcept {
    switch (static_cast<PosixErrorCodes>(c))
    {
        case PosixErrorCodes::InvalidDomain:
            return make_error_condition(std::errc::invalid_argument);
        case PosixErrorCodes::InvalidType:
            return make_error_condition(std::errc::invalid_argument);
    }

    return std::error_condition(c, *this);
}

const katla::PosixErrorCategory &katla::posixSocketErrorCategory() {
    static katla::PosixErrorCategory c;
    return c;
}


