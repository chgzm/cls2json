#include <iostream>
#include <cstdio>
#include <cstdint>
#include <unistd.h>
#include <getopt.h>

#include "ClassFile.h"

static constexpr struct option longopts[] = {
    {0, 0, 0, 0},
};

static void usage() {
    std::printf(
        "Usage: cls2json [OPTIONS] classfile...\n"
    );
}

static int parseCommandLine(int argc, char* argv[], std::vector<std::string>& classFilePaths) noexcept {
    int opt = 0, longIndex = 0;
    while ((opt = getopt_long(argc, argv, "", longopts, &longIndex)) != -1) {
        switch (opt) {
        default: {
            break;
        }
        }
    }

    if (argc <= optind) {
        std::fprintf(stderr, "classfile is required.\n");
        return -1;
    }

    for (int i = optind; i < argc; ++i) {
        classFilePaths.push_back(argv[i]);
    }

    return 0;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        usage();
        return -1;
    }

    std::vector<std::string> classFilePaths;
    if (parseCommandLine(argc, argv, classFilePaths) != 0) {
        return -1;
    }

    for (const std::string& path : classFilePaths) {
        ClassFile classFile;
        if (classFile.load(path) != 0) {
            std::fprintf(stderr, "Failed to load class file \"%s\".\n", path.c_str());
            return -1;
        }

        std::printf("%s\n", classFile.toString().c_str());
    }

    return 0;
}
