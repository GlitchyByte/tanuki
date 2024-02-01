// Copyright 2024 GlitchyByte
// SPDX-License-Identifier: Apache-2.0

#include <iostream>
#include "ImmediateCommand.h"

gb::console::color_t const textColor { gb::console::rgb(1, 1, 1) };

void ImmediateCommand::watchCallback() noexcept {
    if (!action.has_value()) {
        shutdownMonitor->shutdown();
        return;
    }
    std::deque<std::string> lines;
    gb::process::execute(action.value(), nullptr, &lines);
    for (auto const& line: lines) {
        std::cout << gb::console::colorText(line, textColor) << std::endl;
    }
}

void ImmediateCommand::run() noexcept {
    gb::DirectoryWatcher watcher { watchDir, gb::functions::methodWrapper(&ImmediateCommand::watchCallback), this };
    watcher.start();
    shutdownMonitor->awaitShutdown();
    watcher.stop();
}
