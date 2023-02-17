/***
 * Copyright 2022 The Katla Authors
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

#ifndef KATLA_POSIX_PROCESS_H
#define KATLA_POSIX_PROCESS_H

#include "core/observable.h"
#include "katla/core/core.h"
#include "katla/core/error.h"
#include "katla/core/subject.h"

#include "katla/core/posix-pipe.h"
#include "katla/core/signal-handler.h"
#include "katla/core/worker-thread.h"

#include <gsl/span>

#include <optional>
#include <thread>

#include <signal.h>

namespace katla {

class UvPosixProcess {
public:
    enum class Status {Unknown, NotStarted, Starting, Started, Running, Exitted, Signalled, Crashed, Killed};

    UvPosixProcess() = default;
    virtual ~UvPosixProcess();

    struct SpawnOptions {
        bool redirectStdout {false};
    };

    outcome::result<void, Error> spawn(const std::string& path, const std::vector<std::string>& arguments, const std::string& workingDir, const SpawnOptions& options);
    outcome::result<void, Error> kill(Signal signal);

    outcome::result<Status, Error> status();

    outcome::result<ssize_t> readStdout(gsl::span<std::byte>& buffer) {
        return m_fdStdout.read(buffer);
    }

    std::unique_ptr<katla::Subscription>
    onClose(const std::function<void()>& onCloseCallback) {
        return m_onCloseSubject.subscribe(std::make_shared<katla::FuncObserver<void>>(onCloseCallback));
    }

private:
    void handleChildSignal();

    std::optional<int> m_pid {};

    Status m_status {Status::NotStarted};

    katla::PosixPipe m_fdStdout;
    std::unique_ptr<katla::Subscription> m_onChildSubscription;

    katla::Subject<void> m_onCloseSubject;
};

}

#endif // KATLA_PIPE_H
