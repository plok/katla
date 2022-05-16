/***
 * Copyright 2022 The Katla Authors
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

#include "core/string-utils.h"

#include "gtest/gtest.h"

namespace katla {

    TEST(KatlaStringUtilsTests, TrimPrefixTest) {
        const auto *str = "XXXabcXXX";
        auto trimmed = katla::string::trimPrefix(str, "XXX");

        ASSERT_TRUE(trimmed.compare("abcXXX") == 0);        
    }

    TEST(KatlaStringUtilsTests, TrimSuffixTest) {
        const auto *str = "XXXabcXXX";
        auto trimmed = katla::string::trimSuffix(str, "XXX");

        ASSERT_TRUE(trimmed.compare("XXXabc") == 0);        
    }

    TEST(KatlaStringUtilsTests, TrimWhitespaceTest) {
        const auto *str = " abc ";
        auto trimmed = katla::string::trimWhitespace(str);

        ASSERT_TRUE(trimmed.compare("abc") == 0);        
    }

    TEST(KatlaStringUtilsTests, ToUpperTest) {
        const auto *lower = "abcABC";
        auto upper = katla::string::toUpper(lower);

        ASSERT_TRUE(upper.compare("ABCABC") == 0);        
    }

    TEST(KatlaStringUtilsTests, ToLowerTest) {
        const auto *lower = "ABCabc";
        auto upper = katla::string::toLower(lower);

        ASSERT_TRUE(upper.compare("abcabc") == 0);        
    }

    TEST(KatlaStringUtilsTests, StartsWithTest) {
        const auto *str = "abcXXXX";
        ASSERT_TRUE(katla::string::startsWith(str, "abc"));       
    }
    TEST(KatlaStringUtilsTests, StartsWithFailTest) {
        const auto *str = "abcXXXX";
        ASSERT_FALSE(katla::string::startsWith(str, "xyz"));       
    }

    TEST(KatlaStringUtilsTests, EndsWithTest) {
        const auto *str = "XXXXabc";
        ASSERT_TRUE(katla::string::endsWith(str, "abc"));       
    }
    TEST(KatlaStringUtilsTests, EndsWithFailTest) {
        const auto *str = "XXXXabc";
        ASSERT_FALSE(katla::string::endsWith(str, "xyz"));       
    }  
}

