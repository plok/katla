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
#include "fmt/ranges.h"

#include <cstdlib>
#include <exception>
#include <gsl/span>
#include <iostream>
#include <string>
#include <string_view>
#include <type_traits>

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
    template<class ElementType, std::size_t Extent = gsl::dynamic_extent>
    using span = gsl::span<ElementType, Extent>;

    // https://ned14.github.io/outcome/tutorial/essential/no-value/
    template<class R, class S = std::error_code>
#ifdef NDEBUG
    // Any observation of a missing value or error is undefined behavior
    // As bad as this may sound, it generates the most optimal code, and such hard UB is very tool-friendly for
    // detection e.g. undefined behaviour sanitiser, valgrind memcheck, etc.
    // Source: https://ned14.github.io/outcome/tutorial/essential/no-value/builtin/
    using result = outcome::unchecked<R, S>;
#else
    // any observation of a missing value or error throws bad_result_access
    // for easy/early detection in debug only
    using result = outcome::checked<R, S>;
#endif

    // Returns the underlying value of an enumerator
    // Is added, because newer versions (v10+) of fmt do not implicitly convert enums in format string anymore
    template<typename EnumType> [[nodiscard]] constexpr auto enumValue(EnumType enumerator)
    {
        return static_cast<std::underlying_type_t<EnumType>>(enumerator);
    }

    /****
     * Declare format and print here for convenience. Current implementation uses the Fmt lib,
     * the idea is to use std c++ format later. Which is the reason for the indirection.
     */
    template<typename... Args>
    [[nodiscard]] inline std::string format(fmt::format_string<Args...> format_str, Args&&... args)
    {
        try {
            return fmt::format(format_str, std::forward<Args>(args)...);
        } catch (const fmt::format_error& ex) {
            // This exception should not be thrown with >C++17, because of compile time checks
            std::cout << "katla::format format exception (" << ex.what()
                      << ") in: " << std::string_view(format_str.get().data(), format_str.get().size()) << "\n";
        } catch (const std::exception& ex) {
            std::cout << "katla::format exception (" << ex.what() << ")"
                      << "\n";
        } catch (...) {
            std::cout << "katla::format exception"
                      << "\n";
        }
        return { format_str.get().data(), format_str.get().size() };
    }

    template<typename... Args> 
    inline void print(std::FILE* f, fmt::format_string<Args...> format_str, Args&&... args)
    {
        assert(f != nullptr);
        try {
            fmt::print(f, "{}\n", fmt::format(format_str, std::forward<Args>(args)...));
        } catch (const fmt::format_error& ex) {
            // This exception should not be thrown with >C++17, because of compile time checks
            std::cout << "katla::print format exception (" << ex.what()
                      << ") in: " << std::string_view(format_str.get().data(), format_str.get().size()) << "\n";
        } catch (const std::exception& ex) {
            std::cout << "katla::print exception (" << ex.what() << ")"
                      << "\n";
        } catch (...) {
            std::cout << "katla::print exception"
                      << "\n";
        }
    }

    // Wrap fmt::join to return a string. This enables passing it as an argument to katla::format
    // and katla::print (at the cost of an extra dynamic memory allocation)
    template<typename Range> 
    [[nodiscard]] inline std::string join(Range&& range, std::string_view sep)
    {
        try {
            // fmt::join returns a custom string-view like object. This, unfortunately, cannot be used
            // to directly construct a std::string. Therefore fmt::format is used.
            return fmt::format("{}", fmt::join(range, sep));
        } catch (const fmt::format_error& ex) {
            // This exception should not be thrown with >C++17, because of compile time checks
            std::cout << "katla::join format exception (" << ex.what() << ")"
                      << "\n";
        } catch (const std::exception& ex) {
            std::cout << "katla::join exception (" << ex.what() << ")"
                      << "\n";
        } catch (...) {
            std::cout << "katla::join exception"
                      << "\n";
        }
        return {};
    }

    template <typename... Args>
    inline void printInfo(fmt::format_string<Args...> format_str, Args&&... args) {
        katla::print(stdout, format_str, std::forward<Args>(args)...);
        fflush(stdout);
    }

    template <typename... Args>
    inline void printError(fmt::format_string<Args...> format_str, Args&&... args) {
        katla::print(stderr, format_str, std::forward<Args>(args)...);
        fflush(stderr);
    }

    template <typename... Args>
    inline void fatal(fmt::format_string<Args...> format_str, Args&&... args) {
        katla::printError(format_str, std::forward<Args>(args)...);
        std::abort();
    }
}

#endif // KATLA_CORE_H
