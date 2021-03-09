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

#include "posix-pipe.h"

#include <iostream>
#include <vector>

#include <unistd.h>
#include <system_error>

namespace katla {

PosixPipe::PosixPipe()
{
    _fd[0] = -1;
    _fd[1] = -1;
}

PosixPipe::~PosixPipe()
{
    if (_fd[0] != -1) {
        ::close(_fd[0]);
    }
    if (_fd[1] != -1) {
        ::close(_fd[1]);
    }
}

outcome::result<void> PosixPipe::open()
{
    int status = pipe(_fd);
    if (status == -1) {
        return std::make_error_code(static_cast<std::errc>(errno));
    }

    return outcome::success();
}

outcome::result<ssize_t> PosixPipe::read(gsl::span<std::byte>& buffer)
{
    ssize_t nbytes = ::read(_fd[0], buffer.data(), buffer.size());

    if (nbytes == -1) {
        return std::make_error_code(static_cast<std::errc>(errno));
    }

    return nbytes;
}

outcome::result<ssize_t> PosixPipe::write(gsl::span<std::byte>& buffer)
{
    ssize_t nbytes = ::write(_fd[1], buffer.data(), buffer.size());

    if (nbytes == -1) {
        return std::make_error_code(static_cast<std::errc>(errno));
    }

    return nbytes;
}

outcome::result<void> PosixPipe::redirectToRead(int fd_src)
{
    int status = dup2(_fd[0], fd_src);
    if (status != fd_src) {
        return std::make_error_code(static_cast<std::errc>(errno));
    }

    return outcome::success();
}

outcome::result<void> PosixPipe::redirectToWrite(int fd_src)
{
    int status = dup2(_fd[1], fd_src);
    if (status != fd_src) {
        return std::make_error_code(static_cast<std::errc>(errno));
    }

    return outcome::success();
}

outcome::result<void> PosixPipe::close()
{
    auto result1 = closeRead();
    auto result2 = closeWrite();

    // can we aggregate the results?
    if (!result1) {
        return result1;
    }
    if (!result2) {
        return result2;
    }

    return outcome::success();
}

outcome::result<void> PosixPipe::closeRead()
{
    if (_fd[0] == -1) {
        return outcome::success();
    }

    int status = ::close(_fd[0]);
    if (status == -1) {
        return std::make_error_code(static_cast<std::errc>(errno));
    }

    _fd[0] = -1;

    return outcome::success();
}

outcome::result<void> PosixPipe::closeWrite()
{
    if (_fd[1] == -1) {
        return outcome::success();
    }

    int status = ::close(_fd[1]);
    if (status == -1) {
        return std::make_error_code(static_cast<std::errc>(errno));
    }

    _fd[1] = -1;

    return outcome::success();
}

}