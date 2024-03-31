// Copyright 2024 GlitchyByte
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "gb.h"
#ifdef GB_TASK

#include "Task.h"
#include <set>
#include <mutex>
#include <condition_variable>
#include <thread>

namespace gb::concurrent {

    class Task;

    /**
     * Task runner facility to run tasks in their own threads.
     *
     * <p>Note that start method will block until the task signals it has started. This is by design
     * to ensure a task is ready to accept input, for example.
     */
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
        /**
         * Creates a task runner.
         */
        TaskRunner() noexcept;

        /**
         * Destroys the task runner.
         *
         * <p>It shuts down the runner as per shutdown().
         */
        ~TaskRunner() noexcept;

        /**
         * Returns true if the runner is active and able to accept tasks.
         *
         * @return True if the runner is active and able to accept tasks.
         */
        [[nodiscard]]
        bool isActive() const noexcept;

        /**
         * Shuts down the runner.
         *
         * <p>Cancels all tasks and awaits on all tasks to stop.
         */
        void shutdown() noexcept;

        /**
         * Starts a task.
         *
         * <p>This method will block until the task signals it has started.
         *
         * @param task Task to start.
         * @return True if the task was started, false if the runner is not active.
         */
        bool start(std::shared_ptr<Task> const& task) noexcept;

        /**
         * Cancels all tasks.
         */
        void cancelAll() noexcept;

        /**
         * Awaits for all tasks to finish.
         */
        void awaitAll() noexcept;

    private:
        void removeTask(std::shared_ptr<Task> const& task) noexcept;
    };
}

#endif // GB_TASK
