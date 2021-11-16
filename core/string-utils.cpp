#include "string-utils.h"

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
}}
