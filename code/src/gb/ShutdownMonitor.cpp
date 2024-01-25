// Copyright 2023 GlitchyByte
// SPDX-License-Identifier: Apache-2.0

#include "gb/ShutdownMonitor.h"
#ifdef GB_SHUTDOWN_MONITOR

#include <csignal>

namespace gb {

    constinit bool _false { false };
    std::atomic<bool> generalShutdownInitiated { false };
    std::mutex generalShutdownLock;
    std::vector<std::shared_ptr<ShutdownMonitor>> shutdownMonitors;

    void triggerShutdown([[maybe_unused]] int const signal) noexcept {
        if (!generalShutdownInitiated.compare_exchange_strong(_false, true)) {
            return;
        }
        std::lock_guard<std::mutex> lock { generalShutdownLock };
        for (auto const& monitor: shutdownMonitors) {
            monitor->shutdown();
        }
        shutdownMonitors.clear();
    }

    std::shared_ptr<ShutdownMonitor> ShutdownMonitor::create() noexcept {
        static bool initialized { false };
        if (!initialized) {
            std::signal(SIGINT, triggerShutdown);
            std::signal(SIGTERM, triggerShutdown);
            initialized = true;
        }
        std::lock_guard<std::mutex> lock { generalShutdownLock };
        bool const isShuttingDown { generalShutdownInitiated };
        std::shared_ptr<ShutdownMonitor> const monitor { new ShutdownMonitor(isShuttingDown) };
        if (!isShuttingDown) {
            shutdownMonitors.push_back(monitor);
        }
        return monitor;
    }

    bool ShutdownMonitor::shouldShutdown() const noexcept {
        return isShuttingDown;
    }

    void ShutdownMonitor::shutdown() noexcept {
        isShuttingDown = true;
        std::lock_guard<std::mutex> lock { shutdownLock };
        shuttingDown.notify_all();
    }

    void ShutdownMonitor::awaitShutdown(std::chrono::milliseconds const& timeout) noexcept {
        if (shouldShutdown()) {
            return;
        }
        std::unique_lock<std::mutex> lock { shutdownLock };
        std::chrono::time_point<std::chrono::steady_clock> limit { std::chrono::steady_clock::now() + timeout };
        shuttingDown.wait_until(lock, limit, [&] { return isShuttingDown.load(); });
    }

    void ShutdownMonitor::awaitShutdown() noexcept {
        if (shouldShutdown()) {
            return;
        }
        std::unique_lock<std::mutex> lock { shutdownLock };
        shuttingDown.wait(lock, [&] { return isShuttingDown.load(); });
    }

    void ShutdownMonitor::whileLive(std::chrono::milliseconds const& cadence, std::function<void()> const& action) {
        while (!shouldShutdown()) {
            action();
            awaitShutdown(cadence);
        }
    }
}

#endif // GB_SHUTDOWN_MONITOR
