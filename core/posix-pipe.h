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

#ifndef KATLA_PIPE_H
#define KATLA_PIPE_H

#include "katla/core/core.h"

#include <gsl/span>

#include <optional>

namespace outcome = OUTCOME_V2_NAMESPACE;

namespace katla {

class PosixPipe {
public:
    PosixPipe();
    ~PosixPipe();

    outcome::result<void> open();

    outcome::result<ssize_t> read(gsl::span<std::byte>& buffer);
    outcome::result<ssize_t> write(gsl::span<std::byte>& buffer);

    outcome::result<void> redirectToRead(int fd_src);
    outcome::result<void> redirectToWrite(int fd_src);

    outcome::result<void> close();
    outcome::result<void> closeRead();
    outcome::result<void> closeWrite();
private:
    int _fd[2];
};

}

#endif // KATLA_PIPE_H
