#include "string-utils.h"

namespace katla {

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

}
