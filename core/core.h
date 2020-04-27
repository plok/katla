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

#ifndef KATLA_CORE_H
#define KATLA_CORE_H

#include "outcome/outcome.hpp"
namespace outcome = OUTCOME_V2_NAMESPACE;

#include "fmt/format.h"

#include <gsl/span>

namespace katla {

    template<class T, std::size_t Extent>
    using span = gsl::span<T, Extent>;

    /****
     * Declare format and print here for convenience
     */
    template <typename S, typename... Args>
    inline std::string format(const S& format_str, Args&&... args) {
        return fmt::format(format_str, args...);
    }

    template <typename S, typename... Args>
    inline void print(std::FILE* f, const S& format_str, Args&&... args) {
        fmt::print(f, format_str, args...);
    }
}

#endif // KATLA_CORE_H
