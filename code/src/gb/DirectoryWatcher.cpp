// Copyright 2024 GlitchyByte
// SPDX-License-Identifier: Apache-2.0

#include "gb/DirectoryWatcher.h"
#ifdef GB_DIRECTORY_WATCHER

namespace gb {

    std::atomic<uint64_t> queueId { 0 };

    DirectoryWatcher::DirectoryWatcher(std::filesystem::path const& path, WatchCallback const& callback,
            void* callbackContext) noexcept {
        this->paths = std::vector<std::filesystem::path>();
        this->paths.push_back(path);
        this->callback = callback;
        this->callbackContext = callbackContext;
    }

    DirectoryWatcher::~DirectoryWatcher() noexcept {
        stop();
    }

    bool DirectoryWatcher::isWatching() const noexcept {
        return _isWatching;
    }

    void DirectoryWatcher::callCallback() noexcept {
        callback(callbackContext);
    }

#ifdef GB_IS_MACOS
    void eventCallback(ConstFSEventStreamRef const stream, void* const callbackInfo, size_t const eventPathCount,
            void* const eventPaths, FSEventStreamEventFlags const* const eventFlags,
            FSEventStreamEventId const* const eventIds) noexcept {
        auto watcher = static_cast<DirectoryWatcher*>(callbackInfo);
        watcher->callCallback();
    }
#endif

    void DirectoryWatcher::start() noexcept {
        std::unique_lock<std::mutex> lock { watchLock };
        if (isWatching()) {
            return;
        }
#ifdef GB_IS_MACOS
        std::string const queueName { "com.glitchybyte.tanuki.directorywatcher.queue" + std::to_string(queueId++) };
        cfPaths = CFArrayCreateMutable(nullptr, static_cast<CFIndex>(paths.size()), &kCFTypeArrayCallBacks);
        for (auto const& path: paths) {
            CFStringRef cfPath = CFStringCreateWithCString(nullptr, path.c_str(), kCFStringEncodingUTF8);
            CFArrayAppendValue(cfPaths, cfPath);
            CFRelease(cfPath);
        }
        stream = FSEventStreamCreate(nullptr, eventCallback, &streamContext, cfPaths, kFSEventStreamEventIdSinceNow, 2,
                kFSEventStreamCreateFlagNone);
        queue = dispatch_queue_create(queueName.c_str(), DISPATCH_QUEUE_SERIAL);
        FSEventStreamSetDispatchQueue(stream, queue);
        FSEventStreamStart(stream);
#endif
#ifdef GB_IS_LINUX
        inotifyFd = inotify_init();
        if (inotifyFd == -1) {
            return;
        }
        for (auto const& path: paths) {
            int const wd = inotify_add_watch(inotifyFd, path.c_str(), IN_MODIFY | IN_CREATE | IN_DELETE);
            if (wd == -1) {
                close(inotifyFd);
                return;
            }
        }
        if (pipe(cancelPipeFds) == -1) {
            close(inotifyFd);
            return;
        }
        watchThread = std::thread([this]() {
            while (true) {
                fd_set fdset;
                FD_ZERO(&fdset);
                FD_SET(inotifyFd, &fdset);
                FD_SET(cancelPipeFds[0], &fdset);
                int const nfds = std::max(inotifyFd, cancelPipeFds[0]) + 1;
                int const readyFdCount = select(nfds, &fdset, NULL, NULL, NULL);
                if ((readyFdCount == -1) || (FD_ISSET(cancelPipeFds[0], &fdset))) {
                    // Error or cancel signal.
                    break;
                } else if (FD_ISSET(inotifyFd, &fdset)) {
                    char buffer[8 * 1024];
                    auto const bytesRead = read(inotifyFd, buffer, sizeof(buffer));
                    if (bytesRead == -1) {
                        break;
                    }
                    callCallback();
                }
            }
            close(cancelPipeFds[0]);
            close(cancelPipeFds[1]);
            close(inotifyFd);
        });
#endif
        _isWatching = true;
    }

    void DirectoryWatcher::stop() noexcept {
        std::unique_lock<std::mutex> lock { watchLock };
        if (!isWatching()) {
            return;
        }
#ifdef GB_IS_MACOS
        FSEventStreamStop(stream);
        FSEventStreamInvalidate(stream);
        FSEventStreamRelease(stream);
        dispatch_release(queue);
        CFRelease(cfPaths);
#endif
#ifdef GB_IS_LINUX
        char const buffer = 'x';
        [[maybe_unused]] auto result = write(cancelPipeFds[1], &buffer, 1);
        watchThread.join();
#endif
        _isWatching = false;
    }
}

#endif // GB_DIRECTORY_WATCHER
