// Copyright 2024 GlitchyByte
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "gb.h"
#include "CoreCommand.h"

class RunCommand : public CoreCommand {
public:
    explicit RunCommand(std::filesystem::path const& configFile) : CoreCommand(configFile) {}

    void execute() override;
};
