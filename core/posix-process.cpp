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

#include "posix-process.h"

#include "core.h"
#include "error.h"
#include "posix-errors.h"

#include <csignal>
#include <cstdlib>
#include <cstring>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>

namespace katla {

outcome::result<void, Error> PosixProcess::spawn(std::string path, std::vector<std::string> arguments, std::string workingDir)
{
    m_status = Status::Starting;

    auto forkResult = fork();
    if (forkResult == -1) {
        return Error(std::make_error_code(static_cast<std::errc>(errno)), "Failed forking process");
    }

    if (forkResult == 0) {
        // child

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

    m_status = Status::Started;

    status();

    return outcome::success();
}

outcome::result<void, Error> PosixProcess::kill(Signal signal)
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
outcome::result<PosixProcess::Status, Error> PosixProcess::status()
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
    int exitStatus = WEXITSTATUS(status);

    bool signalled = WIFSIGNALED(status) != 0;
    bool crashed = WTERMSIG(status) == SIGSEGV;
    bool killed = WTERMSIG(status) == SIGKILL;

    auto result = PosixProcess::Status::Unknown;
    if (exited) {
        m_status = Status::Exitted;
        // exitStatus
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

}