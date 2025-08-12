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

#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include "core/core.h"

#include <chrono>
#include <string>
#include <vector>

#if __cplusplus <= 201703L
#include "fmt/chrono.h"
#endif

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
namespace string {

    std::string humanFileSize(long long fileSize);

    std::string trimPrefix(std::string src, std::string prefix);

    std::string trimSuffix(const std::string& src, const std::string& suffix);

    std::string trimWhitespace(const std::string& str);

    std::string toUpper(const std::string& str);
    std::string toLower(const std::string& str);

    bool startsWith (const std::string& str, const std::string& start);
    bool endsWith (const std::string& str, const std::string& end);

    bool contains (const std::string& str, const std::string& substr);

    std::vector<std::string> split(std::string input, std::string delimiter);

#if __cplusplus > 201703L
    // Returns 'timePoint' in local time in the specified format (e.g. "{:%Y%m%d-%H%M%S}" -> 20250811-151112) with
    // seconds resolution (by default)
    // Returns an empty string upon failure
    // Note that this function may be relatively slow, because of reading the time zone database, so
    // avoid calling from performance critical functions.
    template<typename Resolution = std::chrono::seconds>
    [[nodiscard]] std::string localTime(std::chrono::system_clock::time_point timePoint,
                                        std::format_string<std::chrono::zoned_time<Resolution>> formatString)
    {
        try {
            return std::format(formatString,
                               std::chrono::zoned_time { std::chrono::current_zone(),
                                                         std::chrono::time_point_cast<Resolution>(timePoint) });
        } catch (const std::runtime_error& ex) {
            katla::printError("Exception determining local time: {}", ex.what());
        }
        return {};
    }
#else
    // Returns 'timePoint' in local time in the specified format (e.g. "{:%Y%m%d-%H%M%S}" -> 20250811-151112) with
    // seconds resolution
    [[nodiscard]] inline std::string localTime(std::chrono::system_clock::time_point timePoint,
                                               fmt::format_string<std::tm> formatString)
    {
        return katla::format(formatString, fmt::localtime(std::chrono::system_clock::to_time_t(timePoint)));
    }
#endif

} // namespace string
} // namespace katla  


#endif // KATLA_CORE_H
