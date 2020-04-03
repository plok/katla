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

#ifndef KATLA_POSIX_FILE_H
#define KATLA_POSIX_FILE_H

#include "absl/types/span.h"

#include "outcome.hpp"

#include <optional>

#include <fcntl.h>

namespace outcome = OUTCOME_V2_NAMESPACE;

namespace katla {

    class PosixFile {
    public:
        enum class OpenFlags : int {
            ReadOnly    = O_RDONLY,
            WriteOnly   = O_WRONLY,
            ReadWrite   = O_RDWR,
            Create      = O_CREAT,
            Exclusive   = O_EXCL,
            Truncate    = O_TRUNC,
            Append      = O_APPEND,
            NonBlocking = O_NONBLOCK,
            Sync        = O_SYNC,
            Async       = O_ASYNC,
            TmpFile     = O_TMPFILE
        };

        PosixFile();
        ~PosixFile();

        outcome::result<void> open(std::string_view filePath, OpenFlags flags, uint32_t mode);
        outcome::result<void> close();

        outcome::result<ssize_t> read(absl::Span<std::byte> &buffer);
        outcome::result<ssize_t> write(absl::Span<std::byte> &buffer);

    private:
        int m_fd;
    };

    inline PosixFile::OpenFlags operator|(PosixFile::OpenFlags lhs, PosixFile::OpenFlags rhs) {
        using T = std::underlying_type_t<PosixFile::OpenFlags>;
        return static_cast<PosixFile::OpenFlags>(static_cast<T>(lhs) | static_cast<T>(rhs));
    }

    inline PosixFile::OpenFlags &operator|=(PosixFile::OpenFlags &lhs, PosixFile::OpenFlags rhs) {
        lhs = lhs | rhs;
        return lhs;
    }

}

#endif // KATLA_POSIX_FILE_H
