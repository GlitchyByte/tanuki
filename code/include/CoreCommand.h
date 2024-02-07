// Copyright 2024 GlitchyByte
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "Command.h"

class CoreCommand : public Command {
public:
    explicit CoreCommand(std::filesystem::path const& configFile);

protected:
    std::filesystem::path configRoot;
    TanukiConfig tanukiConfig;
    std::string summary;

private:
    std::string summarizeWatchConfig() const;
};
