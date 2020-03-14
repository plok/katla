#include "../ipc-kit.h"

#include "app-kit/core-application-uv.h"

#include "uv.h"

#include "../messages.h"

#include "gtest/gtest.h"

#include <iostream>
#include <utility>
#include <memory>
#include <functional>

#include <cstdlib>
#include <unistd.h>
#include <exception>
#include <fmt/format.h>
#include <variant>

namespace katla {

void posixForkAndPipeChild(int fd) {
    write(fd, "Hello world\n", 12);
    close(fd);
}

void posixForkAndPipeParent(int fd) {
    const int BUFFER_SIZE = 100;
    char buf[BUFFER_SIZE];
    ssize_t nbytes = read(fd, buf, BUFFER_SIZE); 
    close(fd);

    EXPECT_EQ(nbytes, 12);
}

TEST(KatlaIpcTests, PosixForkAndPipe) {
    int fildes[2];
    auto pipeStatus = pipe(fildes);
    if (pipeStatus == -1) {
        auto pipeErrno = errno;
        EXPECT_NE(pipeErrno, 0);
        ASSERT_NE(pipeStatus, -1);
    }

    auto forkResult = fork();
    if (forkResult == -1) {
        ASSERT_NE(forkResult, -1);
    }

    if (forkResult == 0) {
        // child
        close(fildes[0]);                      
        posixForkAndPipeChild(fildes[1]);

        exit(EXIT_SUCCESS);
    }

    // parent
    close(fildes[1]);                            
    posixForkAndPipeParent(fildes[0]);
}

}
