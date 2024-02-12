// Copyright 2023-2024 GlitchyByte
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "gb.h"
#ifdef GB_SHUTDOWN_MONITOR

#include <atomic>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <functional>

namespace gb {

    /**
     * Utility to monitor SIGINT and SIGTERM for proper application shutdown.
     */
    class ShutdownMonitor {
    public:
        /**
         * Creates a monitor that will get notified when it's time for an orderly shutdown.
         *
         * @return A shutdown monitor.
         */
        [[nodiscard]]
        static std::shared_ptr<ShutdownMonitor> create() noexcept;

        /**
         * Returns true when an orderly shutdown should occur.
         *
         * @return True when an orderly shutdown should occur.
         */
        [[nodiscard]]
        bool shouldShutdown() const noexcept;

        /**
         * Manually triggers an orderly shutdown.
         */
        void shutdown() noexcept;

        /**
         * Awaits for a shutdown or expiration of the given timeout.
         * <p>
         * If a shutdown has been triggered, the method will exit fast.
         *
         * @param timeout Time to wait for shutdown.
         */
        void awaitShutdown(std::chrono::milliseconds const& timeout) noexcept;

        /**
         * Awaits for a shutdown.
         * <p>
         * If a shutdown has been triggered, the method will exit fast.
         */
        void awaitShutdown() noexcept;

        /**
         * Convenience method to execute an action periodically at the given cadence,
         * until a shutdown is triggered.
         *
         * @param cadence Cadence at which to execute the action.
         * @param action Action to execute.
         */
        void whileLive(std::chrono::milliseconds const& cadence, std::function<void()> const& action);


    private:
        std::atomic<bool> isShuttingDown { false };
        std::mutex shutdownLock;
        std::condition_variable shuttingDown;

        explicit ShutdownMonitor(bool const shuttingDown) noexcept : isShuttingDown(shuttingDown) {}
    };
}

#endif // GB_SHUTDOWN_MONITOR
