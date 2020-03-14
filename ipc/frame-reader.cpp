#include "frame-reader.h"

#include "messages.h"

#include <fmt/format.h>

#include <iostream>
#include <vector>


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
