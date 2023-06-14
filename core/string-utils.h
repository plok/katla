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

} // namespace string
} // namespace katla  


#endif // KATLA_CORE_H
