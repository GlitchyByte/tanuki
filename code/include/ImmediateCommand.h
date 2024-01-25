// Copyright 2024 GlitchyByte
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "gb.h"
#include "Command.h"

class ImmediateCommand : public Command {
public:
    explicit ImmediateCommand(std::filesystem::path const& watchDir, std::optional<std::string> const& action) noexcept
        : watchDir(watchDir), action(action) {};

    void run() noexcept override;

private:
    std::filesystem::path const watchDir;
    std::optional<std::string> const action;
};
