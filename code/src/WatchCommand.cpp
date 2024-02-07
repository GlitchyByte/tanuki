// Copyright 2024 GlitchyByte
// SPDX-License-Identifier: Apache-2.0

#include "WatchRunTask.h"
#include "Colors.h"
#include "WatchCommand.h"
#include <vector>
#include <filesystem>
#include <thread>
#include <iostream>

void WatchCommand::printRunningModule(TanukiConfigModule const& module) noexcept {
    std::cout << gb::terminal::colorText("Running", Colors::heading)
        << ": "
        << gb::terminal::colorText(module.name, Colors::highlight)
        << std::endl;
}

void WatchCommand::printTimeSeparator() noexcept {
    std::time_t const now = std::time(nullptr);
    std::string const time = gb::strings::fromTime(now, "%I:%M:%S %p");
    std::cout << std::string(60, '-')
        << "< "
        << gb::terminal::colorText(time, Colors::highlight)
        << " >----"
        << std::endl;
}

void startWatch(std::filesystem::path const& configRoot, TanukiConfigModule const& module, std::string const& summary) {
    WatchRunTask task { configRoot, module, summary };
    task.watch();
}

void WatchCommand::execute() {
    printTimeSeparator();
    std::cout << summary << std::endl;
    std::vector<std::thread> threads;
    for (auto const& module: tanukiConfig.modules) {
        threads.emplace_back(startWatch, configRoot, module, summary);
    }
    for (std::thread& thread: threads) {
        thread.join();
    }
}
