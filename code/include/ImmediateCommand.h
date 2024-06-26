// Copyright 2024 GlitchyByte
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "gb.h"
#include "Command.h"

class ImmediateCommand : public Command {
private:
    std::shared_ptr<gb::ShutdownMonitor> shutdownMonitor { gb::ShutdownMonitor::create() };
    std::filesystem::path const watchDir;
    std::optional<std::string> const action;

public:
    explicit ImmediateCommand(std::filesystem::path const& watchDir, std::optional<std::string> const& action) noexcept
        : watchDir(watchDir), action(action) {};

    void execute() override;

private:
    void watchCallback() noexcept;
};
