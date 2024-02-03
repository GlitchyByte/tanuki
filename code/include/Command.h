// Copyright 2024 GlitchyByte
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <filesystem>

class Command {
public:
    virtual void execute() = 0;

    virtual ~Command() = default;

protected:
    void runAction(std::string const& action, std::filesystem::path const* workDir) noexcept;
};
