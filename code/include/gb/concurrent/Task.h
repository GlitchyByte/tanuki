// Copyright 2024 GlitchyByte
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "gb.h"
#ifdef GB_TASK

#include <mutex>
#include <condition_variable>
#include <thread>

namespace gb::concurrent {

    class TaskRunner;

    enum class TaskState : int {
        Created,
        Started,
        Canceled,
        Finished
    };

    /**
     * A single concurrent task that starts in a separate thread and runs until
     * finished or the task is canceled.
     */
    class Task {
        friend class TaskRunner;

    private:
        static std::atomic<uint64_t> nextTaskId;

    protected:
        std::mutex stateLock;

    private:
        uint64_t const taskId;
        std::thread thread;
        TaskRunner* runner { nullptr };
        std::atomic<TaskState> state { TaskState::Created };
        std::condition_variable stateChangedSignal;
        std::atomic<bool> _shouldCancel { false };

    public:
        Task() noexcept : taskId(nextTaskId++) {};

        virtual ~Task() noexcept = default;

        TaskState getState() const noexcept;

        void cancel() noexcept;

        void awaitStop() noexcept;

        bool isStopped() const noexcept;

    protected:
        virtual void action() noexcept = 0;

        [[nodiscard]]
        TaskRunner* getTaskRunner() const noexcept;

        void started() noexcept;

        [[nodiscard]]
        bool shouldCancel() const noexcept;

    private:
        void setTaskRunner(TaskRunner* const newRunner) noexcept;

        void canceled() noexcept;

        void finished() noexcept;

        void awaitState(TaskState const desiredState) noexcept;

        void awaitStart() noexcept;
    };
}

#endif // GB_TASK
