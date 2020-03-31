#ifndef KATLA_PROCESS_H
#define KATLA_PROCESS_H

#include "katla-ipckit.h"

#include "process.h"

#include "process-uv.h"

#include "app-kit/event-loop-uv.h"
#include "uv.h"

#include "common/error.h"
#include <memory>
#include <string>
#include <vector>

class KATLA_IPCKIT_DECLSPEC UvProcess {
public:
    explicit UvProcess(UvEventLoop& eventLoop);
    virtual ~UvProcess();
    
    bool run(std::string fileName, std::vector<std::string> args);
private:
    UvEventLoop* _eventLoop;

    std::string _file;
    std::string _cwd;

    uv_process_t _process;
    uv_process_options_t _options;

    uv_pipe_t _pipe;
    uv_pipe_t _pipe2;

    std::vector<uv_stdio_container_t> _stdioContainer;

    std::vector<std::string> _cppArgs;
    std::vector<char*> _args;
};

#endif
