// Copyright 2024 GlitchyByte
// SPDX-License-Identifier: Apache-2.0

#include <iostream>
#include "ImmediateCommand.h"

void ImmediateCommand::run() noexcept {
    std::cout << watchDir << std::endl;
    std::cout << action.value_or("<none>") << std::endl;
    bool alert = true;
    while (alert) {
        alert = gb::system::watchDir(watchDir);
    }
}
