#ifndef FORMAT_H
#define FORMAT_H

#include <string>

template <typename... Args>
std::string fmt(const char* format, Args... args) noexcept {
    const std::size_t len = std::snprintf(nullptr, 0, format, args...);
    char buf[len + 1];
    std::snprintf(buf, len + 1, format, args...);

    return std::string(buf);
}

#endif
