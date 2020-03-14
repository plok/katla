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

#include "app-kit/core-application-uv.h"

#include "ipc/ipc-kit.h"
#include "ipc/process-uv.h"

#include "string.h"
#include <iostream>
#include <vector>

int main(int argc, char* argv[])
{
    std::vector<std::string> args;
    for (int i=0; i<argc; i++) {
        std::string arg = argv[i];
        args.push_back(arg);
    }

    std::cout << "Starting katla-ipc-test with args: ";
    for(auto s : args)
        std::cout << s; std::cout << std::endl;

    UvCoreApplication app;
    auto appError = app.init();
    if (appError) {
        std::cout << appError->name << " " << appError->message << std::endl << std::flush;
        return EXIT_FAILURE;
    }
	
    IpcKit ipcKit(app);
    ipcKit.init();
   
    std::vector<std::string> childArgs = {};

    UvProcess process (*app.uvEventLoop());
    process.run("katla-ipc-test-child", childArgs);

    app.run();

    std::cout << std::flush;

    return 0;
}
