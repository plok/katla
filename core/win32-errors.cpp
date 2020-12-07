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
#include "win32-errors.h"

std::string katla::Win32ErrorCategory::message(int c) const {
    switch (static_cast<Win32ErrorCodes>(c))
    {
        case Win32ErrorCodes::OperationFailed:
            return "Operation Failed";
    }

    return "unknown";
}

std::error_condition katla::Win32ErrorCategory::default_error_condition(int c) const noexcept {
    switch (static_cast<Win32ErrorCodes>(c))
    {
        case Win32ErrorCodes::OperationFailed:
            return std::error_condition(c, *this);
            break;
    }

    return std::error_condition(c, *this);
}

const katla::Win32ErrorCategory &katla::win32SocketErrorCategory() {
    static katla::Win32ErrorCategory c;
    return c;
}


