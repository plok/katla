#include "process-uv.h"

#include <iostream>
#include <utility>
#include <cstring>

void UvProcessExitCallback(uv_process_t*, int64_t exit_status, int term_signal)
{
    std::cout << "Process exited <status, signal>: " << exit_status << " " << term_signal << std::endl;
}

UvProcess::UvProcess(UvEventLoop& eventLoop) :
    _eventLoop(&eventLoop),
    _process(),
    _options()
{
}

UvProcess::~UvProcess() = default;

void on_read_alloc(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf)
{
    std::cout << "on read alloc" << std::endl;

    buf->base = reinterpret_cast<char*>(malloc(suggested_size));
    buf->len = suggested_size;
}

void on_read(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf)
{
    uv_write_t* req;
    uv_buf_t wrbuf;
    int r;

    //ASSERT(nread > 0 || nread == UV_EOF);

    std::cout << "on read" << std::endl;

    if (nread > 0) {
        std::string input;
        input.resize(buf->len);
        strncpy (input.data(), buf->base, buf->len);

        std::cout << "child: " << input << std::endl;
    }

    //if (nread > 0) {
    //output_used += nread;
    //if (output_used == 12) {
      //ASSERT(memcmp("hello world\n", output, 12) == 0);
      //wrbuf = uv_buf_init(output, output_used);
      //req = malloc(sizeof(*req));
      //r = uv_write(req, (uv_stream_t*)&in, &wrbuf, 1, after_write);
      //ASSERT(r == 0);
    //}
    //}
}

bool UvProcess::run(std::string fileName, std::vector<std::string> args)
{
    _file = fileName;
    _cwd = "/home/simon/projects/katla/katla/build/out"; // TODO currentDir

    _options.file = _file.c_str();
    _options.cwd = _cwd.c_str();

    _cppArgs = args;

    _args.clear();
    for (auto arg : _cppArgs)
    {
        _args.push_back(const_cast<char*>(arg.c_str()));
    }
    _args.push_back(nullptr);
    

    _options.args = _args.data();

    _stdioContainer.clear();

    uv_pipe_init(_eventLoop->handle(), &_pipe, 1);
    uv_pipe_init(_eventLoop->handle(), &_pipe2, 1);

    uv_stdio_container_t in;
    in.flags = UV_IGNORE;
    in.data.fd = 0;

    uv_stdio_container_t out;
    out.flags = static_cast<uv_stdio_flags>(UV_CREATE_PIPE | UV_WRITABLE_PIPE);
    out.data.stream = reinterpret_cast<uv_stream_t*>(&_pipe);

    uv_stdio_container_t child_out2;
    child_out2.flags = static_cast<uv_stdio_flags>(UV_CREATE_PIPE | UV_WRITABLE_PIPE);
    child_out2.data.stream = reinterpret_cast<uv_stream_t*>(&_pipe2);

    _stdioContainer.push_back(in);
    _stdioContainer.push_back(out);
    _stdioContainer.push_back(child_out2);

    _options.stdio_count = 3;
    _options.stdio = _stdioContainer.data();

    int result = uv_spawn(_eventLoop->handle(), &_process, &_options);

    auto r = uv_read_start(reinterpret_cast<uv_stream_t*>(&_pipe), on_read_alloc, on_read);
    auto r2 = uv_read_start(reinterpret_cast<uv_stream_t*>(&_pipe2), on_read_alloc, on_read);

    std::cout << result << " - " << _process.pid << std::endl;
    // TODO parse error code

    return result == 0;
}


