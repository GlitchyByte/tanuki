// Copyright 2024 GlitchyByte
// SPDX-License-Identifier: Apache-2.0

#include "WatchRunTask.h"
#include "Colors.h"
#include "WatchCommand.h"
#include <thread>
#include <iostream>

std::random_device WatchCommand::randomDevice;
std::mt19937 WatchCommand::random { randomDevice() };
std::uniform_int_distribution<int> WatchCommand::positionRange { 0, 59 };

void WatchCommand::printTimeSeparator() noexcept {
    std::time_t const now = std::time(nullptr);
    std::string const time = gb::strings::fromTime(now, "%I:%M:%S %p");
    int const n1 { positionRange(random) };
    int const n2 { positionRange(random) };
    int const n3 { positionRange(random) };
    std::ostringstream os;
    os << std::string(60, '-')
        << "< "
        << gb::terminal::colorText(time, Colors::highlight)
        << " >----";
    std::string line { os.str() };
    line[n1] = 'o';
    line[n2] = 'o';
    line[n3] = 'O';
    std::cout << line << std::endl;
}

void WatchCommand::execute() {
    // Run all.
    for (auto const& module: tanukiConfig.modules) {
        runModule(configRoot, module);
    }
    // Watch all.
    printTimeSeparator();
    std::cout << summary << std::endl;
    std::shared_ptr<gb::ShutdownMonitor> shutdownMonitor { gb::ShutdownMonitor::create() };
    gb::concurrent::TaskRunner runner;
    for (auto const& module: tanukiConfig.modules) {
        std::shared_ptr<WatchRunTask> task { std::make_shared<WatchRunTask>(configRoot, module, summary) };
        runner.start(task);
    }
    shutdownMonitor->awaitShutdown();
    runner.shutdown();
}
