// Copyright 2024 GlitchyByte
// SPDX-License-Identifier: Apache-2.0

#include "Colors.h"
#include "CoreCommand.h"
#include <fstream>

CoreCommand::CoreCommand(std::filesystem::path const& configFile) {
    configRoot = configFile.parent_path();
    std::ifstream file { configFile };
    nlohmann::json data;
    file >> data;
    data.get_to(tanukiConfig);
    summary = summarizeWatchConfig();
}

std::string CoreCommand::summarizeWatchConfig() const {
    std::ostringstream ss;
    ss << gb::terminal::colorText("Watching", Colors::heading) << ':' << std::endl;
    for (auto const& module: tanukiConfig.modules) {
        ss << gb::terminal::colorText("§", Colors::heading) << ' ' << gb::terminal::colorText(module.name, Colors::highlight);
        std::filesystem::path moduleDir { configRoot };
        moduleDir.append(module.rootDir);
        if (!std::filesystem::is_directory(moduleDir)) {
            std::ostringstream ess;
            ess << "In module \"" << module.name << "\" rootDir \"" << module.rootDir << "\" doesn't exist!";
            throw std::invalid_argument(ess.str());
        }
        ss << " (" << gb::terminal::colorText(moduleDir.string(), Colors::text) << ')' << std::endl;
        std::vector<std::filesystem::path> paths;
        for (auto const& subPath: module.watchDirs) {
            ss << "  " << gb::terminal::colorText("»", Colors::highlight)
                << ' ' << gb::terminal::colorText(subPath, Colors::text) << std::endl;
            std::filesystem::path path { moduleDir };
            path.append(subPath);
            if (!std::filesystem::is_directory(path)) {
                std::ostringstream ess;
                ess << "In module \"" << module.name << "\" watchDir \"" << subPath << "\" doesn't exist!";
                throw std::invalid_argument(ess.str());
            }
            paths.push_back(path);
        }
    }
    return ss.str();
}
