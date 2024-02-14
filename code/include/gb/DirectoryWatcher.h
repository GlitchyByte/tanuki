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

namespace gb {

    using WatchCallback = std::function<void(void*)>;

    /**
     * Directory watcher.
     */
    class DirectoryWatcher {
    public:
        explicit DirectoryWatcher(std::vector<std::filesystem::path> const& paths, WatchCallback const& callback,
                void* callbackContext = nullptr) noexcept : paths(paths), callback(callback), callbackContext(callbackContext) {};

        explicit DirectoryWatcher(std::filesystem::path const& path, WatchCallback const& callback,
                void* callbackContext = nullptr) noexcept;

        ~DirectoryWatcher() noexcept;

        [[nodiscard]]
        bool isWatching() const noexcept;

        void callCallback() noexcept;

        void start() noexcept;

        void stop() noexcept;

    private:
        std::vector<std::filesystem::path> paths;
        WatchCallback callback;
        void* callbackContext;
        std::mutex watchLock;
        std::atomic<bool> _isWatching { false };
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
    };
}

#endif // GB_DIRECTORY_WATCHER
