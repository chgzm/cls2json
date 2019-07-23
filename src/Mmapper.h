#ifndef MMAPPER_H
#define MMAPPER_H

#include <cstdint>
#include <string>

class Mmapper {
public:
    Mmapper()  noexcept;
    ~Mmapper() noexcept;

    void* mmapReadOnly(const std::string& filePath) noexcept;

private:
    std::size_t fileSize_;
    void*       addr_;  
};

#endif
