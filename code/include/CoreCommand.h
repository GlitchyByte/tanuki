// Copyright 2024 GlitchyByte
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "Command.h"

class CoreCommand : public Command {
protected:
    std::filesystem::path configRoot;
    TanukiConfig tanukiConfig;
    std::string summary;

public:
    explicit CoreCommand(std::filesystem::path const& configFile);

private:
    [[nodiscard]]
    std::string summarizeWatchConfig() const;
};
