// Copyright 2024 GlitchyByte
// SPDX-License-Identifier: Apache-2.0

#pragma once

class Command {
public:
    virtual void execute() noexcept = 0;

    virtual ~Command() = default;
};
