// Copyright 2024 GlitchyByte
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "gb.h"
#include "Command.h"

class WatchCommand : public CoreCommand {
public:
    explicit WatchCommand() noexcept;

    void run() noexcept override;
};
