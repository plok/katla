#include "core/core.h"
#include "core/string-utils.h"

#include "gtest/gtest.h"

#include <chrono>

namespace katla {
TEST(KatlaCoreTests, FormatAndPrint)
{
#if __cplusplus > 201703L
    // Missing argument results in compiler error with >C++17
    // katla::printInfo("{} {}", "arg1");
#else
    // Missing argument results in exception with <=C++17
    katla::printInfo("Test string with 2 arguments, but only 1 passed {} {}", "arg1");
#endif

    katla::print(stdout, "print to stdout via print");
    katla::printInfo("print to stdout via printInfo");
    katla::print(stderr, "print to stderr via print");
    katla::printError("print to stderr via printError");
    // katla::fatal("fatal error");

    std::vector<int> numbers = { 1, 2, 3, 4, 5 };
    katla::printInfo("join numbers: {}", katla::join(numbers, ", "));
    katla::printInfo("numbers (w/o join): {}", numbers);

    std::array<std::string, 3> arrayStrings = { "str1", "str2", "str3" };
    katla::printInfo("join arrayStrings: {}", katla::join(arrayStrings, " & "));
    katla::printInfo("arrayStrings (w/o join): {}", arrayStrings);

    static constexpr char dateFormatString[] = "{:%Y%m%d-%H%M%S}";

    auto timePoint = std::chrono::system_clock::now();

    katla::printInfo("timestamp (precison: default, sec): {}", string::localTime(timePoint, dateFormatString));
#if __cplusplus > 201703L
    katla::printInfo("timestamp (precison: ms): {}",
                     string::localTime<std::chrono::milliseconds>(timePoint, dateFormatString));
    katla::printInfo("timestamp (precison: ns): {}",
                     string::localTime<std::chrono::nanoseconds>(timePoint, dateFormatString));
    katla::printInfo("timestamp (minutes format): {}",
                     string::localTime<std::chrono::nanoseconds>(timePoint, "{:%Y%m%d-%H%M}"));
#endif
}
} // namespace katla
