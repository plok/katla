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

#include "posix-file.h"
#include "gsl/gsl_util"

#include <system_error>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <climits>

namespace katla {

PosixFile::PosixFile() {}

PosixFile::~PosixFile()
{
    if (m_fd != -1) {
        ::close(m_fd);
    }
}

outcome::result<void> PosixFile::create(std::string_view pathPath, PosixFile::OpenFlags flags)
{
    m_fd = ::open(std::string(pathPath).c_str(), gsl::narrow_cast<uint>(flags) | gsl::narrow_cast<uint>(O_CLOEXEC), 0644);
    if (m_fd == -1) {
        return std::make_error_code(static_cast<std::errc>(errno));
    }

    return outcome::success();
}

outcome::result<void> PosixFile::open(std::string_view pathPath, PosixFile::OpenFlags flags)
{
    m_fd = ::open(std::string(pathPath).c_str(), static_cast<int>(flags), 0644);
    if (m_fd == -1) {
        return std::make_error_code(static_cast<std::errc>(errno));
    }

    return outcome::success();
}

outcome::result<void> PosixFile::close()
{
    if (m_fd == -1) {
        return outcome::success();
    }

    int status = ::close(m_fd);
    if (status == -1) {
        return std::make_error_code(static_cast<std::errc>(errno));
    }

    m_fd = -1;

    return outcome::success();
}

outcome::result<ssize_t> PosixFile::read(gsl::span<std::byte>& buffer)
{
    ssize_t nbytes = ::read(m_fd, buffer.data(), buffer.size());

    if (nbytes == -1) {
        return std::make_error_code(static_cast<std::errc>(errno));
    }

    return nbytes;
}

outcome::result<ssize_t> PosixFile::write(gsl::span<std::byte>& buffer)
{
    ssize_t nbytes = ::write(m_fd, buffer.data(), buffer.size());

    if (nbytes == -1) {
        return std::make_error_code(static_cast<std::errc>(errno));
    }

    return nbytes;
}

outcome::result<std::string> PosixFile::absolutePath(std::string path) {
    char actualpath [PATH_MAX+1];
    auto cstrPtr = realpath(path.c_str(), actualpath);
    if (cstrPtr == nullptr) {
        return std::make_error_code(static_cast<std::errc>(errno));
    }

    return std::string(cstrPtr);
}


outcome::result<size_t> PosixFile::size()
{
    struct stat fileInfo;
    int result = fstat(m_fd, &fileInfo);
    if (result == -1) {
        return std::make_error_code(static_cast<std::errc>(errno));
    }

    return fileInfo.st_size;
}

}
