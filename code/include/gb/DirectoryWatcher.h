// Copyright 2024 GlitchyByte
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "gb.h"
#ifdef GB_DIRECTORY_WATCHER

#include <vector>
#include <atomic>
#include <mutex>
#ifdef GB_IS_MACOS
#include <CoreServices/CoreServices.h>
#endif
#ifdef GB_IS_LINUX
#include <sys/inotify.h>
#include <thread>
#endif
#ifdef GB_IS_WINDOWS
#include <thread>
#include <windows.h>
#endif

namespace gb {

    using WatchCallback = std::function<void(void*)>;

    /**
     * Directory watcher facility to react to changes in a directory.
     */
    class DirectoryWatcher {
    private:
#ifdef GB_IS_MACOS
        static std::atomic<uint64_t> queueId;
#endif

    private:
        class CallbackRunnerTask;

    private:
        std::vector<std::filesystem::path> paths;
        WatchCallback callback;
        void* callbackContext;
        std::mutex watchLock;
        std::atomic<bool> _isWatching { false };
        std::unique_ptr<gb::concurrent::TaskRunner> runner { std::make_unique<gb::concurrent::TaskRunner>() };
        std::mutex actionLock;
        std::shared_ptr<CallbackRunnerTask> actionTask;
#ifdef GB_IS_MACOS
        CFMutableArrayRef cfPaths;
        FSEventStreamContext streamContext { 0, this, nullptr, nullptr, nullptr };
        FSEventStreamRef stream;
        dispatch_queue_t queue;
#endif
#ifdef GB_IS_LINUX
        std::thread watchThread;
        int inotifyFd;
        int cancelPipeFds[2];
#endif
#ifdef GB_IS_WINDOWS
        std::thread watchThread;
        OVERLAPPED watcherContext { .Pointer = this };
        HANDLE hCancelEvent { NULL };
#endif

    public:
        /**
         * Creates a directory watcher.
         *
         * <p>It doesn't start watching and calling the callback until start() is issued.
         *
         * @param paths Collection of paths to observe.
         * @param callback Callback method to execute when a change is observed.
         * @param callbackContext Callback object on which the callback method will be executed.
         */
        explicit DirectoryWatcher(std::vector<std::filesystem::path> const& paths, WatchCallback const& callback,
                void* callbackContext = nullptr) noexcept : paths(paths), callback(callback), callbackContext(callbackContext) {};

        /**
         * Creates a directory watcher.
         *
         * <p>It doesn't start watching and calling the callback until start() is issued.
         *
         * @param path Path to observe.
         * @param callback Callback method to execute when a change is observed.
         * @param callbackContext Callback object on which the callback method will be executed.
         */
        explicit DirectoryWatcher(std::filesystem::path const& path, WatchCallback const& callback,
                void* callbackContext = nullptr) noexcept;

        ~DirectoryWatcher() noexcept;

        /**
         * Returns true if this watcher is active.
         *
         * @return True if this watcher is active.
         */
        [[nodiscard]]
        bool isWatching() const noexcept;

        /**
         * Internal use only! Calls the callback when a change is observed.
         */
        void callCallback() noexcept;

        /**
         * Starts watching for changes.
         */
        void start() noexcept;

        /**
         * Stops watching for changes.
         */
        void stop() noexcept;
    };
}

#endif // GB_DIRECTORY_WATCHER
