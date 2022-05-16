#include "string-utils.h"
#include <algorithm>
#include <cctype>

namespace katla {
namespace string {

std::string humanFileSize(long long fileSize) {
    auto suffixes = std::array{"B", "KiB", "MiB", "GiB", "TiB"};
    auto suffixIndex = 0;
    
    double size = fileSize;
    while(size > 1024. && suffixIndex < (suffixes.size()-1)) {
        size = size / 1024.;
        suffixIndex++;
    }

    return format("{:.2f}{}", size, suffixes[suffixIndex]);
}

std::string trimPrefix(std::string src, std::string prefix) {
    auto pos = src.rfind(prefix, 0);
    if (pos != std::string::npos) {
        auto result = src;
        result.erase(0, prefix.size());
        return result;
    }

    return src;
}

std::string trimSuffix(const std::string& src, const std::string& suffix)
{
    if (suffix.empty()) {
        return src;
    }
    const size_t srcLength = src.length();
    const size_t suffixLength = suffix.length();
    if (suffixLength > srcLength) {
        return src;
    }

    std::string srcCopy(src);
    if (srcCopy.compare(srcLength - suffixLength, suffixLength, suffix) == 0) {
        srcCopy.erase(srcLength - suffixLength);
    }
    return srcCopy;
}

std::string trimWhitespace(const std::string& str)
{
    std::string result = str;
    result.erase(result.begin(),
                 std::find_if(result.begin(), result.end(), [](unsigned char ch) { return std::isspace(ch) == 0; }));
    result.erase(
        std::find_if(result.rbegin(), result.rend(), [](unsigned char ch) { return std::isspace(ch) == 0; }).base(),
        result.end());
    return result;
}

std::string toUpper(const std::string& str)
{
    std::string result = str;

    auto tfunc = [](char c) -> char {
        return std::toupper(c);
    };
    std::transform(result.begin(), result.end(), result.begin(), tfunc);
    return result;
}

std::string toLower(const std::string& str)
{
    std::string result = str;

    auto tfunc = [](char c) -> char {
        return static_cast<int>(std::tolower(c));
    };
    std::transform(result.begin(), result.end(), result.begin(), tfunc);
    return result;
}

bool startsWith (const std::string& str, const std::string& start)
{
    if (start.length() > str.length()) {
        return false;
    }

    return (str.compare(0, start.length(), start) == 0);
}

bool endsWith (const std::string& str, const std::string& end) {
    if (end.length() > str.length()) {
        return false;
    }

    return (str.compare (str.length() - end.length(), end.length(), end) == 0);
}


}  // namespace string
}  // namespace katla
