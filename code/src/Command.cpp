// Copyright 2024 GlitchyByte
// SPDX-License-Identifier: Apache-2.0

#include "CoreCommand.h"
#include "gb.h"
#include "Colors.h"
#include <iostream>

void Command::runAction(std::string const& action, std::filesystem::path const* workDir) noexcept {
    if (action.empty()) {
        return;
    }
    std::deque<std::string> lines;
    gb::process::execute(action, workDir, &lines);
    for (auto const& line: lines) {
        std::cout << gb::console::colorText(line, Colors::textColor) << std::endl;
    }
}
