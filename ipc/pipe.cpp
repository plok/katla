#include "pipe.h"

#include <iostream>
#include <vector>

#include <unistd.h>
#include <system_error>

Pipe::Pipe()
{
    _fd[0] = -1;
    _fd[1] = -1;
}

Pipe::~Pipe()
{
    if (_fd[0] != -1) {
        ::close(_fd[0]);
    }
    if (_fd[1] != -1) {
        ::close(_fd[1]);
    }
}

outcome::result<void> Pipe::open()
{
    int status = pipe(_fd);
    if (status == -1) {
        return std::make_error_code(static_cast<std::errc>(errno));
    }

    return outcome::success();
}

outcome::result<ssize_t> Pipe::read(absl::Span<std::byte>& buffer)
{
    ssize_t nbytes = ::read(_fd[0], buffer.data(), buffer.size());

    if (nbytes == -1) {
        return std::make_error_code(static_cast<std::errc>(errno));
    }

    return nbytes;
}

outcome::result<ssize_t> Pipe::write(absl::Span<std::byte>& buffer)
{
    ssize_t nbytes = ::write(_fd[1], buffer.data(), buffer.size());

    if (nbytes == -1) {
        return std::make_error_code(static_cast<std::errc>(errno));
    }

    return nbytes;
}

outcome::result<void> Pipe::close()
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

outcome::result<void> Pipe::closeRead()
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

outcome::result<void> Pipe::closeWrite()
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

