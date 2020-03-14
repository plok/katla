#ifndef FRAME_READER_H
#define FRAME_READER_H

#include "absl/types/span.h"

#include <optional>

/***
 * Frame level
 * - TODO multiplexing using streams?
 * - TODO version handshaking (for only frame level??)
 * - TODO priority
 * - ?? support self-describing message formats?
 * - ?? on which level support message formats?
 * - ?? multiplexing
 */
class FrameReader {
public:
    FrameReader() = default;
    std::optional<std::vector<std::byte>> read(absl::Span<std::byte>& data);
private:
    // TODO use ringbuffer
    std::vector<std::byte> _data;
};

#endif // FRAME_READER_H
