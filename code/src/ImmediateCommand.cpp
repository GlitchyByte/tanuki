// Copyright 2024 GlitchyByte
// SPDX-License-Identifier: Apache-2.0

#include "ImmediateCommand.h"

void ImmediateCommand::watchCallback() noexcept {
    if (!action.has_value()) {
        shutdownMonitor->shutdown();
        return;
    }
    runAction(action.value(), nullptr);
}

void ImmediateCommand::execute() {
    gb::DirectoryWatcher watcher { watchDir, gb::functions::methodWrapper(&ImmediateCommand::watchCallback), this };
    watcher.start();
    shutdownMonitor->awaitShutdown();
    watcher.stop();
}
