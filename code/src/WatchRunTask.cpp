// Copyright 2024 GlitchyByte
// SPDX-License-Identifier: Apache-2.0

#include "Command.h"
#include "WatchCommand.h"
#include "WatchRunTask.h"
#include <filesystem>
#include <iostream>

std::mutex callbackLock;

WatchRunTask::WatchRunTask(std::filesystem::path const& configRoot, TanukiConfigModule const& module,
        std::string const& summary) noexcept {
    this->configRoot = configRoot;
    this->module = module;
    this->summary = summary;
    std::filesystem::path moduleDir { configRoot };
    moduleDir.append(module.rootDir);
    for (auto const& subPath: module.watchDirs) {
        std::filesystem::path path { moduleDir };
        path.append(subPath);
        paths.push_back(path);
    }
}

void WatchRunTask::watch() noexcept {
    gb::DirectoryWatcher watcher { paths, gb::functions::methodWrapper(&WatchRunTask::watchCallback), this };
    watcher.start();
    shutdownMonitor->awaitShutdown();
    watcher.stop();
}

void WatchRunTask::watchCallback() noexcept {
    std::lock_guard<std::mutex> lock { callbackLock };
    WatchCommand::printRunningModule(module);
    Command::runModule(configRoot, module);
    WatchCommand::printTimeSeparator();
    std::cout << summary << std::endl;
}
