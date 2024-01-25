// Copyright 2023-2024 GlitchyByte
// SPDX-License-Identifier: Apache-2.0

#include "gb/system.h"
#ifdef GB_SYSTEM

#include <iostream>
#include <CoreServices/CoreServices.h>

namespace gb::system {

    void callback(ConstFSEventStreamRef const stream, void* const callbackInfo, size_t const eventPathCount,
            void* const eventPaths, FSEventStreamEventFlags const* const eventFlags,
            FSEventStreamEventId const* const eventIds) {
        std::cout << "changed! " << eventPathCount << std::endl;
    }

    bool watchDirs(std::vector<std::filesystem::path> const& paths) noexcept {
        CFMutableArrayRef cfPaths = CFArrayCreateMutable(nullptr, static_cast<CFIndex>(paths.size()), &kCFTypeArrayCallBacks);
        for (auto const& path: paths) {
            CFStringRef cfPath = CFStringCreateWithCString(nullptr, path.c_str(), kCFStringEncodingUTF8);
            CFArrayAppendValue(cfPaths, cfPath);
            CFRelease(cfPath);
        }
        FSEventStreamRef stream = FSEventStreamCreate(nullptr, &callback, nullptr, cfPaths,
                kFSEventStreamEventIdSinceNow, 2, kFSEventStreamCreateFlagNone);
        dispatch_queue_t queue = dispatch_queue_create("com.glitchybyte.tanuki.queue", DISPATCH_QUEUE_SERIAL);
        FSEventStreamSetDispatchQueue(stream, queue);
        FSEventStreamStart(stream);

        gb::ShutdownMonitor::create()->awaitShutdown();

        FSEventStreamStop(stream);
        FSEventStreamInvalidate(stream);
        FSEventStreamRelease(stream);
        dispatch_release(queue);
        CFRelease(cfPaths);

        return false;
    }

    bool watchDir(std::filesystem::path const& path) noexcept {
        std::vector<std::filesystem::path> paths;
        paths.push_back(path);
        return watchDirs(paths);
    }
}

#endif // GB_SYSTEM
