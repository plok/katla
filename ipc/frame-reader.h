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
