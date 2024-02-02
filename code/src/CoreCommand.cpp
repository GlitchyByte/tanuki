// Copyright 2024 GlitchyByte
// SPDX-License-Identifier: Apache-2.0

#include "CoreCommand.h"
#include <fstream>

CoreCommand::CoreCommand(std::filesystem::path const& configFile) {
    configRoot = configFile.parent_path();
    std::ifstream file { configFile };
    nlohmann::json data;
    file >> data;
    data.get_to(tanukiConfig);
}

void CoreCommand::run() noexcept {
    //
}
