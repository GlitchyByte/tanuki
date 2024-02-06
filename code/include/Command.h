// Copyright 2024 GlitchyByte
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "TanukiConfig.h"
#include <filesystem>

class Command {
public:
    static void runAction(std::string const& action, std::filesystem::path const* workDir) noexcept;

    static void runModule(std::filesystem::path const& configRoot, TanukiConfigModule const& module) noexcept;

    virtual void execute() = 0;

    virtual ~Command() = default;
};
