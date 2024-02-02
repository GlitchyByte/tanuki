// Copyright 2024 GlitchyByte
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "external/nlohmann/json.hpp"

class TanukiConfigModule {
public:
    std::string name;
    std::string rootDir;
    std::vector<std::string> watchDirs;
    std::string action;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(TanukiConfigModule, name, rootDir, watchDirs, action)

class TanukiConfig {
public:
    std::vector<TanukiConfigModule> modules;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(TanukiConfig, modules)
