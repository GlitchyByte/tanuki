// Copyright 2024 GlitchyByte
// SPDX-License-Identifier: Apache-2.0

#include "gb/DirectoryWatcher.h"
#ifdef GB_DIRECTORY_WATCHER

namespace gb {

#ifdef GB_IS_MACOS
    std::atomic<uint64_t> DirectoryWatcher::queueId { 0 };
#endif

    class DirectoryWatcher::CallbackRunnerTask : public gb::concurrent::Task {
    private:
        DirectoryWatcher* watcher;
        std::chrono::time_point<std::chrono::system_clock> timeToCall;

    public:
        explicit CallbackRunnerTask(DirectoryWatcher* const watcher) noexcept;

        bool resetTimer() noexcept;

    protected:
        void action() noexcept override;
    };

    DirectoryWatcher::CallbackRunnerTask::CallbackRunnerTask(DirectoryWatcher* const watcher) noexcept {
        this->watcher = watcher;
        resetTimer();
    }

    bool DirectoryWatcher::CallbackRunnerTask::resetTimer() noexcept {
        std::lock_guard<std::mutex> lock { stateLock };
        if (isStopped()) {
            return false;
        }
        std::chrono::time_point<std::chrono::system_clock> const now = std::chrono::system_clock::now();
        std::chrono::seconds const oneSecond { 1 };
        timeToCall = now + oneSecond;
        return true;
    }

    void DirectoryWatcher::CallbackRunnerTask::action() noexcept {
        started();
        while (!shouldCancel()) {
            std::chrono::time_point<std::chrono::system_clock> const now = std::chrono::system_clock::now();
            if (timeToCall <= now) {
                watcher->callback(watcher->callbackContext);
                return;
            }
            std::this_thread::sleep_until(timeToCall);
        }
    }

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
        std::lock_guard<std::mutex> lock { actionLock };
        if (!actionTask || !actionTask->resetTimer()) {
            actionTask = std::make_shared<CallbackRunnerTask>(this);
            runner->start(actionTask);
        }
    }

#ifdef GB_IS_MACOS
    void eventCallback(ConstFSEventStreamRef const stream, void* const callbackInfo, size_t const eventPathCount,
            void* const eventPaths, FSEventStreamEventFlags const* const eventFlags,
            FSEventStreamEventId const* const eventIds) noexcept {
        auto watcher = static_cast<DirectoryWatcher*>(callbackInfo);
        watcher->callCallback();
    }
#endif
#ifdef GB_IS_WINDOWS
    void CALLBACK eventCallback(DWORD dwErrorCode, DWORD dwNumberOfBytesTransfered, LPOVERLAPPED lpOverlapped) {
        if (dwErrorCode != ERROR_SUCCESS) {
            // Something bad happened, but we don't capture. It works or it doesn't.
            return;
        }
        auto watcher = static_cast<DirectoryWatcher*>(lpOverlapped->Pointer);
        watcher->callCallback();
    }
#endif

#ifdef GB_IS_LINUX
    bool DirectoryWatcher::initializeFds() noexcept {
        if (pipe(cancelPipeFds) == -1) {
            return false;
        }
        inotifyFd = inotify_init();
        if (inotifyFd == -1) {
            return false;
        }
        for (auto const& path: paths) {
            int const wd = inotify_add_watch(inotifyFd, path.c_str(), IN_MODIFY | IN_CREATE | IN_DELETE);
            if (wd == -1) {
                return false;
            }
        }
        epollFd = epoll_create1(0);
        if (epollFd == -1) {
            return false;
        }
        epoll_event event { .events = EPOLLIN, .data { .fd = cancelPipeFds[0] } };
        if (epoll_ctl(epollFd, EPOLL_CTL_ADD, cancelPipeFds[0], &event) == -1) {
            return false;
        }
        event.data.fd = inotifyFd;
        if (epoll_ctl(epollFd, EPOLL_CTL_ADD, inotifyFd, &event) == -1) {
            return false;
        }
        return true;
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
        if (!initializeFds()) {
            if (cancelPipeFds[0] != 0) {
                close(cancelPipeFds[0]);
            }
            if (cancelPipeFds[1] != 0) {
                close(cancelPipeFds[1]);
            }
            if (inotifyFd != 0) {
                close(inotifyFd);
            }
            if (epollFd != 0) {
                close(epollFd);
            }
            return;
        }
        watchThread = std::thread([this]() {
            epoll_event events[16];
            while (true) {
                int const readyFdCount = epoll_wait(epollFd, events, 16, -1);
                if (readyFdCount == -1) {
                    if (errno == EINTR) {
                        // Supurious interrupt! We continue.
                        continue;
                    }
                    break;
                }
                for (int i = 0; i < readyFdCount; ++i) {
                    if (events[i].data.fd == cancelPipeFds[0]) {
                        // User exit signal.
                        goto exitThread;
                    } else if (events[i].data.fd == inotifyFd) {
                        char buffer[8 * 1024];
                        auto const bytesRead = read(inotifyFd, buffer, sizeof(buffer));
                        if (bytesRead == -1) {
                            goto exitThread;
                        }
                        callCallback();
                    }
                }
            }
            exitThread:
            close(cancelPipeFds[0]);
            close(cancelPipeFds[1]);
            close(inotifyFd);
            close(epollFd);
        });
#endif
#ifdef GB_IS_WINDOWS
        hCancelEvent = CreateEvent(NULL, true, false, NULL);
        watchThread = std::thread([this]() {
            std::vector<HANDLE> handles;
            handles.reserve(paths.size() + 1);
            handles.push_back(hCancelEvent);
            for (auto const& path: paths) {
                HANDLE hDir = CreateFileW(path.wstring().c_str(), FILE_LIST_DIRECTORY,
                        FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, NULL, OPEN_EXISTING,
                        FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED, NULL);
                if (hDir == INVALID_HANDLE_VALUE) {
                    return;
                }
                handles.push_back(hDir);
                if (!ReadDirectoryChangesW(hDir, NULL, 0, true,
                        FILE_NOTIFY_CHANGE_LAST_WRITE | FILE_NOTIFY_CHANGE_FILE_NAME, NULL, &watcherContext,
                        eventCallback)
                ) {
                    for (auto const& handle: handles) {
                        CloseHandle(handle);
                    }
                    return;
                }
            }
            while (true) {
                DWORD const result = WaitForMultipleObjectsEx(handles.size(), handles.data(), false, INFINITE, true);
                if (result == WAIT_FAILED) {
                    break;
                }
                if (result == WAIT_OBJECT_0) {
                    // Cancel event is at index 0. We exit if it's set.
                    break;
                }
                if ((result > WAIT_OBJECT_0) && (result < (MAXIMUM_WAIT_OBJECTS - WAIT_OBJECT_0))) {
                    int const index = result - WAIT_OBJECT_0;
                    HANDLE hDir = handles[index];
                    if (!ReadDirectoryChangesW(hDir, NULL, 0, true,
                            FILE_NOTIFY_CHANGE_LAST_WRITE | FILE_NOTIFY_CHANGE_FILE_NAME, NULL, &watcherContext,
                            eventCallback)
                    ) {
                        break;
                    }
                }
            }
            for (auto const& handle: handles) {
                CloseHandle(handle);
            }
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
#ifdef GB_IS_WINDOWS
        SetEvent(hCancelEvent);
        watchThread.join();
#endif
        _isWatching = false;
    }
}

#endif // GB_DIRECTORY_WATCHER
