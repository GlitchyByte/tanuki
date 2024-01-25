// Copyright 2023-2024 GlitchyByte
// SPDX-License-Identifier: Apache-2.0

#include "gb/files.h"
#ifdef GB_FILES

#include <string>

namespace gb::files {

    std::filesystem::path canonicalPath(std::filesystem::path const& path) noexcept {
        std::string pathStr { path.string() };
        std::string const fullPath {
            pathStr.starts_with("~/")
            #ifdef _WIN32
            || pathStr.starts_with("~\\")
            #endif
            ?
                std::getenv("HOME") + pathStr.substr(2) :
                std::move(pathStr)
        };
        return std::filesystem::weakly_canonical(fullPath);
    }
}

#endif // GB_FILES
