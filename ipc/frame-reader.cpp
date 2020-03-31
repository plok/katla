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

#include "frame-reader.h"

#include "messages.h"

#include <fmt/format.h>

#include <iostream>
#include <vector>

// TODO dont create vector, pass buffer as argument for efficiency
std::optional<std::vector<std::byte> > FrameReader::read(absl::Span<std::byte> &data)
{
    if (data.size())
    {
        std::cout << "framereader: insert!" << std::endl;
        _data.insert(_data.end(), data.begin(), data.end());
    }

//    constexpr int minSize = std::min(sizeof(InitialHeader), sizeof(DataHeader));
    if (_data.size() >= sizeof(FrameHeader))
    {
        FrameHeader* frameHeader = reinterpret_cast<FrameHeader*>(data.data());
        if (data.size() >= frameHeader->frameLength) {
            std::vector<std::byte> frameBytes(data.begin(), data.begin() + frameHeader->frameLength);
            _data.erase(_data.begin(), _data.begin()+frameHeader->frameLength);
            return frameBytes;
        }

//        int frameType = std::to_integer<int>(_data[0]);

//        int length = 0;
//        if (frameType == static_cast<uint8_t>(FrameType::Initial) && data.size() >= sizeof(InitialFrame)) {
//            InitialFrame* frame = reinterpret_cast<InitialFrame*>(data.data());

//            std::cout << fmt::format("framereader: initial-frame received: {0}!", frameType) << std::endl;

//            length = sizeof(InitialFrame) + frame->header.payloadLength;

//            std::vector<std::byte> frameBytes(data.begin(), data.begin() + length);
//            _data.erase(_data.begin(), _data.begin()+length);
//            return frameBytes;
//        }

//        if (frameType == static_cast<uint8_t>(FrameType::Data) && data.size() >= sizeof(DataHeader)) {
//            std::cout << fmt::format("framereader: data-frame received: {0}!", frameType) << std::endl;

//            DataFrame* frame = reinterpret_cast<DataFrame*>(data.data());
//            length = sizeof(DataFrame) + static_cast<uint32_t>(frame->header.payloadLength);

//            std::vector<std::byte> frameBytes(data.begin(), data.begin() + length);
//            _data.erase(_data.begin(), _data.begin()+length);
//            return frameBytes;
//        }

    }

    return {};
}
