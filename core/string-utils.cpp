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

void trimSuffix(std::string& src, const std::string& suffix)
{
    if (suffix.empty()) {
        return;
    }
    const size_t srcLength = src.length();
    if (srcLength >= suffix.length()) {
        if (src.compare(srcLength - suffix.length(), suffix.length(), suffix) == 0) {
            src.erase(srcLength - suffix.length());
        }
    }
}
}}
