// Copyright 2024 GlitchyByte
// SPDX-License-Identifier: Apache-2.0

#include "CoreCommand.h"
#include "gb.h"
#include <fstream>
#include <sstream>
#include <iostream>

CoreCommand::CoreCommand(std::filesystem::path const& configFile) {
    configRoot = configFile.parent_path();
    std::ifstream file { configFile };
    nlohmann::json data;
    file >> data;
    data.get_to(tanukiConfig);
}

void CoreCommand::runAll() {
    for (auto const& module: tanukiConfig.modules) {
        run(module);
    }
}

void CoreCommand::run(TanukiConfigModule const& module) {
    std::filesystem::path moduleDir { configRoot };
    moduleDir.append(module.rootDir);
    moduleDir = gb::files::canonicalPath(moduleDir);
    if (!std::filesystem::is_directory(moduleDir)) {
        std::ostringstream ss;
        ss << "In module \"" << module.name << "\" rootDir \"" << module.rootDir << "\" doesn't exist!";
        throw std::invalid_argument(ss.str());
    }
    runAction(module.action, &moduleDir);
}
