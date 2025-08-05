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

#include <cstdio>
#include <cstdlib>
#include <string>

#ifdef _MSC_VER
    #ifdef KATLA_CORE_INDLL
    # define KATLA_CORE_DECLSPEC __declspec(dllexport)
    #else
    # define KATLA_CORE_DECLSPEC __declspec(dllimport)
    #endif
#else
    #define KATLA_CORE_DECLSPEC
#endif

namespace katla {

    template<class T, std::size_t Extent = gsl::dynamic_extent>
    using span = gsl::span<T, Extent>;

    template<class R, class S = std::error_code>
    using result = outcome::result<R, S>;

    /****
     * Declare format and print here for convenience. Current implementation uses the Fmt lib,
     * the idea is to use std c++ format later. Which is the reason for the indirection.
     */
    template <typename S, typename... Args>
    inline std::string format(const S& format_str, Args&&... args) {
        try {
            return fmt::format(format_str, args...);
        }
        catch (...) {
            // Gracefully handle thrown exceptions, which may occur when fewer arguments are
            // passed than required by the format string. This helps to avoid run-time crashes,
            // which could occur when formatting logs and errors for exceptional scenarios.
            //
            // This won't be needed after switching to std::format, as it will raise an error
            // at compile-time instead.

            return format_str;
        }
    }

    template <typename S, typename... Args>
    inline void print(std::FILE* f, const S& format_str, Args&&... args) {
        // Use print (as a means to output char[], std::string, and other string types to file)
        // but do not use its built-in formatting on the passed format string to avoid exceptions.
        fmt::print(f, "{}", format(format_str, args...));
    }

    // Wrap fmt::join to return a string. This enables passing it as an argument to katla::format
    // and katla::print (at the cost of an extra dynamic memory allocation)
    template <typename Range, typename S>
    inline std::string join(Range&& range, S sep) {
        // fmt::join returns a custom string-view like object. This, unfortunately, cannot be used
        // to directly construct a std::string. Therefore ftm::format is used.
        return fmt::format("{}", fmt::join(std::begin(range), std::end(range), sep));
    }

    template <typename S, typename... Args>
    inline void printInfo(const S& message, Args&&... args) {
        print(stdout, fmt::format("{}\n", message), args...);
        fflush(stdout);
    }

    template <typename S, typename... Args>
    inline void printError(const S& message, Args&&... args) {
        print(stderr, fmt::format("{}\n", message), args...);
        fflush(stderr);
    }

    template <typename S, typename... Args>
    inline void fatal(const S& message, Args&&... args) {
        print(stderr, fmt::format("{}\n", message), args...);
        fflush(stderr);
        std::abort();
    }
}

#endif // KATLA_CORE_H
