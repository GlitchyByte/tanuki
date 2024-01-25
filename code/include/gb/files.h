// Copyright 2023 GlitchyByte
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "gb.h"
#ifdef GB_FILES

#include <filesystem>

namespace gb::files {

    /**
     * Expands user home directory if path starts with '~', and normalizes path.
     *
     * @param path Path to canonicalize.
     * @return Canonical path.
     */
    [[nodiscard]]
    std::filesystem::path canonicalPath(std::filesystem::path const& path) noexcept;
}

#endif // GB_FILES
