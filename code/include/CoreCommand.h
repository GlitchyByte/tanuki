// Copyright 2024 GlitchyByte
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "Command.h"
#include "TanukiConfig.h"
#include <filesystem>

class CoreCommand : public Command {
public:
    explicit CoreCommand(std::filesystem::path const& configFile);

protected:
    void runAll();
    void run(TanukiConfigModule const& module);

    std::filesystem::path configRoot;
    TanukiConfig tanukiConfig;
};
