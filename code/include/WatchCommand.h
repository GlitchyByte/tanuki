// Copyright 2024 GlitchyByte
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "gb.h"
#include "CoreCommand.h"

class WatchCommand : public CoreCommand {
public:
    static void printRunningModule(TanukiConfigModule const& module) noexcept;
    static void printTimeSeparator() noexcept;

    explicit WatchCommand(std::filesystem::path const& configFile) : CoreCommand(configFile) {};

    void execute() override;
};
