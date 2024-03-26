// Copyright 2024 GlitchyByte
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "gb.h"
#include "CoreCommand.h"
#include <random>

class WatchCommand : public CoreCommand {
public:
    static std::random_device randomDevice;
    static std::mt19937 random;
    static std::uniform_int_distribution<int> positionRange;

public:
    static void printTimeSeparator() noexcept;

    explicit WatchCommand(std::filesystem::path const& configFile) : CoreCommand(configFile) {};

    void execute() override;
};
