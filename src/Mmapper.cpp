#include "Mmapper.h"

#include <cstdio>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

Mmapper::Mmapper() noexcept
  : fileSize_(0),
    addr_(nullptr) {
}

Mmapper::~Mmapper() noexcept {
    if (this->addr_ != nullptr && this->fileSize_ != 0) {
        if (munmap(this->addr_, this->fileSize_) != 0) {
            std::fprintf(stderr, "munmap failed.\n");
        }
    }
}

void* Mmapper::mmapReadOnly(const std::string& filePath) noexcept {
    const int fd = open(filePath.c_str(), O_RDONLY);
    if (fd < 0) {
        std::fprintf(stderr, "open failed.\n");
        return nullptr;
    }

    struct stat sb;
    if (fstat(fd, &sb) == -1) {
        std::fprintf(stderr, "fstat failed.\n");
        return nullptr;
    }
    this->fileSize_ = sb.st_size;

    this->addr_ = mmap(nullptr, this->fileSize_, PROT_READ, MAP_PRIVATE, fd, 0);
    if (this->addr_ == MAP_FAILED) {
        std::fprintf(stderr, "mmap failed\n");
        return nullptr;
    }

    if (close(fd) == -1) {
        std::fprintf(stderr, "close failed.\n");
        return nullptr;
    }

    return this->addr_;
}
