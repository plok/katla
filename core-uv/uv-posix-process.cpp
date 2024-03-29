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

#include "katla/core-uv/uv-posix-process.h"

#include "katla/core/core.h"
#include "katla/core/error.h"
#include "katla/core/posix-errors.h"
#include "katla/core-uv/uv-core-application.h"

#include <csignal>
#include <cstdlib>
#include <cstring>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>
#include <chrono>

namespace katla {

UvPosixProcess::~UvPosixProcess() {
    if (m_onChildSubscription) {
        m_onChildSubscription->unsubscribe();
    }
}

outcome::result<void, Error> UvPosixProcess::spawn(const std::string& path, const std::vector<std::string>& arguments, const std::string& workingDir, const SpawnOptions& options)
{
    m_status = Status::Starting;

    if (UvCoreApplication::hasInstance()) {
        m_onChildSubscription = UvCoreApplication::instance().onChild([this](){
            handleChildSignal();
        });
    } else {
        katla::printInfo("Warning: no UvCoreApplication instance present. onClose event will be non-functional");
    }

    if (options.redirectStdout) {
        auto openPipeOut = m_fdStdout.open();
        if (!openPipeOut) {
            return Error(std::make_error_code(static_cast<std::errc>(errno)), "Failed openeing pipe");
        }
    }

    auto forkResult = fork();
    if (forkResult == -1) {
        return Error(std::make_error_code(static_cast<std::errc>(errno)), "Failed forking process");
    }

    if (forkResult == 0) {
        // child
        if (options.redirectStdout) {
            auto closeReadResult = m_fdStdout.closeRead();
            if (!closeReadResult) {
                katla::printError("Failed closing read-end of fdStdout: {}", closeReadResult.error().message());
            }

            auto redirectOutResult = m_fdStdout.redirectToWrite(STDOUT_FILENO);
            if (!redirectOutResult) {
                katla::printError("Error connecting pipe to stdout: {}'", redirectOutResult.error().message());
                exit(EXIT_FAILURE);
            }
        }

        if (!workingDir.empty() && workingDir != "./") {
            int chdirResult = chdir(workingDir.c_str());
            if (chdirResult != 0) {
                katla::printError(fmt::format("Error setting working directory: {}'", strerror(errno)));
                exit(EXIT_FAILURE);
            }
        }

        std::vector<char*> args;
        args.push_back(const_cast<char*>(path.c_str()));
        for(auto& it : arguments) {
            args.push_back(const_cast<char*>(it.c_str()));
        }
        args.push_back(nullptr);

        if (execv(path.c_str(), args.data()) != 0) {
            katla::printError(fmt::format(fmt::format("Error starting child process: {}'", strerror(errno))));
            exit(EXIT_FAILURE);
        }

        // unreachable
        assert(false);
    } 

    // parent
    m_pid = forkResult;
    if (options.redirectStdout) {
        auto closeWriteResult = m_fdStdout.closeWrite();
        if (!closeWriteResult) {
            katla::printError("Failed closing write-end of fdStdout: {}", closeWriteResult.error().message());
        }
    }

    m_status = Status::Started;

    auto _ = status();

    return outcome::success();
}

outcome::result<void, Error> UvPosixProcess::kill(Signal signal)
{
    if (!m_pid.has_value()) {
        return Error(make_error_code(katla::PosixErrorCodes::Invalid), "No process active!");
    }

    int sig = 0;
    switch (signal) {
    case Signal::Unknown: assert(false); break;
    case Signal::Interrupt: sig = SIGINT; break;
    case Signal::Hangup: sig = SIGHUP; break;
    case Signal::Kill: sig = SIGKILL; break;
    case Signal::Stop: sig = SIGSTOP; break;
    case Signal::Terminate: sig = SIGTERM; break;
    case Signal::Child: sig = SIGCHLD; break;
    }

    int res = ::kill(m_pid.value(), sig);
    if (res != 0) {
        return Error(std::make_error_code(static_cast<std::errc>(errno)), "Failed killing process");;
    }

    // TODO waitpid so resources can be released, block or no block??
    // TODO close stdin stdout

    return outcome::success();
}

// status needs to be called by parent after stopping process
outcome::result<UvPosixProcess::Status, Error> UvPosixProcess::status()
{
    if (!m_pid.has_value()) {
        return Error(make_error_code(katla::PosixErrorCodes::Invalid), "No process active!");
    }

    int status = 0;
    int waitResult = waitpid(m_pid.value(), &status, WNOHANG);

    if (waitResult == -1 && errno == ECHILD) {
        // m_status should already be set
        switch (m_status) {
        case Status::Starting:
        case Status::Started:
        case Status::Running:
            m_status = Status::Unknown;
            katla::printInfo("Unexpected child state: {} {}", errno, strerror(errno));
            return Error(std::make_error_code(static_cast<std::errc>(errno)),
                     katla::format("Unexpected child state: {} {}",  errno, strerror(errno)));
        default: {
        }
        }

        return m_status;
    }

    if (waitResult == -1) {
        katla::printInfo("Failed getting process status: {} {}", errno, strerror(errno));
        return Error(std::make_error_code(static_cast<std::errc>(errno)),
                     katla::format("Failed getting process status: {}", strerror(errno)));
    }

    if (waitResult == 0) {
        if (m_status == Status::Started) {
            m_status = Status::Running;
        } else if (m_status != Status::Running) {
            katla::printInfo("Unexpected child state: {}, expected Running", m_status);
            m_status = Status::Running;
        }

        return m_status;
    }

    bool exited = WIFEXITED(status) != 0;

    bool signalled = WIFSIGNALED(status) != 0;
    bool crashed = WTERMSIG(status) == SIGSEGV;
    bool killed = WTERMSIG(status) == SIGKILL;

    if (exited) {
        m_status = Status::Exitted;

        // TODO implement return of process exit status
        // int exitStatus = WEXITSTATUS(status);
    }
    if (signalled) {
        m_status = Status::Signalled;
    }
    if (signalled && crashed) {
        m_status = Status::Crashed;
    }
    if (signalled && killed) {
        m_status = Status::Killed;
    }

    return m_status;
}

void UvPosixProcess::handleChildSignal()
{
    if (!m_pid.has_value()) {
        return;
    }

    // onClose event will only trigger when no status() is called yet!
    if (m_status != Status::Started && m_status != Status::Running) {
        return;
    }

    auto _ = status();

    if (m_status != Status::Started && m_status != Status::Running) {
        m_onCloseSubject.next();
    }
}

}