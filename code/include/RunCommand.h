// Copyright 2024 GlitchyByte
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "gb.h"
#include "Command.h"

class RunCommand : public CoreCommand {
public:
    explicit RunCommand() noexcept;

    void run() noexcept override;
};
