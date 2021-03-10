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

#include "image/c-api/image.h"

#include "katla/core/core.h"

#include "gtest/gtest.h"

namespace katla {

    TEST(KatlaImageTests, ReadLenna) {

        auto image = read_image("lenna.png");
        ASSERT_TRUE(image != nullptr);

        auto linestride = image_line_stride(image);

        destroy_image(image);

        ASSERT_TRUE(linestride == 2048);
    }

    TEST(KatlaImageTests, WriteLenna) {

        auto image = read_image("lenna.png");
        ASSERT_TRUE(image != nullptr);

        auto linestride = image_line_stride(image);

        write_image_sync("lenna-write-test-out.png", image);

        destroy_image(image);

        ASSERT_TRUE(linestride == 2048);
    }

    TEST(KatlaImageTests, CopyRoi) {

        auto image = read_image("lenna.png");
        ASSERT_TRUE(image != nullptr);

        auto linestride = image_line_stride(image);

        Rect_32s rect {10,10,100,100};
        
        auto roi = create_image_from_existing_roi(image, rect);

        write_image_sync("lenna-copy-roi-test-out.png", roi);

        destroy_image(image);

        ASSERT_TRUE(linestride == 2048);
    }
}

