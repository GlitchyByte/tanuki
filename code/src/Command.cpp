// Copyright 2024 GlitchyByte
// SPDX-License-Identifier: Apache-2.0

#include "Command.h"
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
        std::cout << gb::terminal::colorText(line, Colors::text) << std::endl;
    }
}

void Command::runModule(std::filesystem::path const& configRoot, TanukiConfigModule const& module) noexcept {
    std::cout << gb::terminal::colorText("Running", Colors::heading)
            << ": "
            << gb::terminal::colorText(module.name, Colors::highlight)
            << " (" << gb::terminal::colorText(module.action, Colors::text) << ')'
            << std::endl;
    std::filesystem::path moduleDir { configRoot };
    moduleDir.append(module.rootDir);
    moduleDir = gb::files::canonicalPath(moduleDir);
    runAction(module.action, &moduleDir);
}
