// Copyright 2024 GlitchyByte
// SPDX-License-Identifier: Apache-2.0

#include "RunCommand.h"

void RunCommand::execute() {
    // Run all.
    for (auto const& module: tanukiConfig.modules) {
        runModule(configRoot, module);
    }
}
