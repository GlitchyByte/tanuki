// Copyright 2024 GlitchyByte
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "gb.h"
#ifdef GB_SYSTEM

#include <filesystem>

namespace gb::system {

    [[nodiscard]]
    bool watchDirs(std::vector<std::filesystem::path> const& paths) noexcept;

    [[nodiscard]]
    bool watchDir(std::filesystem::path const& path) noexcept;
}

#endif // GB_SYSTEM
