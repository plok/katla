#ifndef KATLA_PIPE_H
#define KATLA_PIPE_H

#include "absl/types/span.h"

#include "outcome.hpp"

#include <optional>

namespace outcome = OUTCOME_V2_NAMESPACE;

class Pipe {
public:
    Pipe();
    ~Pipe();

    outcome::result<void> open();

    outcome::result<ssize_t> read(absl::Span<std::byte>& buffer);
    outcome::result<ssize_t> write(absl::Span<std::byte>& buffer);

    outcome::result<void> close();
    outcome::result<void> closeRead();
    outcome::result<void> closeWrite();
private:
    int _fd[2];
};

#endif // KATLA_PIPE_H
