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

    void eventCallback(ConstFSEventStreamRef const stream, void* const callbackInfo, size_t const eventPathCount,
            void* const eventPaths, FSEventStreamEventFlags const* const eventFlags,
            FSEventStreamEventId const* const eventIds) noexcept {
        auto watcher = static_cast<DirectoryWatcher*>(callbackInfo);
        watcher->callCallback();
    }

    void DirectoryWatcher::start() noexcept {
        std::unique_lock<std::mutex> lock { watchLock };
        if (isWatching()) {
            return;
        }
        queueName = "com.glitchybyte.tanuki.directorywatcher.queue" + std::to_string(queueId++);
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
        _isWatching = true;
    }

    void DirectoryWatcher::stop() noexcept {
        std::unique_lock<std::mutex> lock { watchLock };
        if (!isWatching()) {
            return;
        }
        FSEventStreamStop(stream);
        FSEventStreamInvalidate(stream);
        FSEventStreamRelease(stream);
        dispatch_release(queue);
        CFRelease(cfPaths);
        _isWatching = false;
    }
}

#endif // GB_DIRECTORY_WATCHER
