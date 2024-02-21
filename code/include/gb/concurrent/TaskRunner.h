// Copyright 2024 GlitchyByte
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "gb.h"
#ifdef GB_TASK

#include "Task.h"
#include <set>
#include <atomic>
#include <mutex>
#include <thread>

namespace gb::concurrent {

    class Task;

    class TaskRunner {
    private:
        static bool taskComparator(std::shared_ptr<Task> const& lhs, std::shared_ptr<Task> const& rhs) noexcept;

    private:
        std::atomic<bool> _isActive { true };
        std::set<std::shared_ptr<Task>, decltype(&taskComparator)> tasks { taskComparator };
        std::mutex tasksLock;
        std::condition_variable isEmptySignal;
        std::thread threadController;
        std::mutex threadControllerLock;
        bool threadControllerShouldExit { false };
        std::vector<std::shared_ptr<Task>> finishingTasks;
        std::condition_variable threadControllerWakeUpSignal;

    public:
        TaskRunner() noexcept;

        ~TaskRunner() noexcept;

        [[nodiscard]]
        bool isActive() const noexcept;

        void shutdown() noexcept;

        bool start(std::shared_ptr<Task>& task) noexcept;

        void cancelAll() noexcept;

    private:
        void removeTask(std::shared_ptr<Task>& task) noexcept;
    };
}

#endif // GB_TASK
