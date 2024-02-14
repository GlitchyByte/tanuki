// Copyright 2024 GlitchyByte
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "gb.h"
#include "TanukiConfig.h"

class WatchRunTask {
private:
    std::shared_ptr<gb::ShutdownMonitor> shutdownMonitor { gb::ShutdownMonitor::create() };
    std::filesystem::path configRoot;
    TanukiConfigModule module;
    std::string summary;
    std::vector<std::filesystem::path> paths;

public:
    explicit WatchRunTask(std::filesystem::path const& configRoot, TanukiConfigModule const& module,
            std::string const& summary) noexcept;

    void watch() noexcept;

private:
    void watchCallback() noexcept;
};
