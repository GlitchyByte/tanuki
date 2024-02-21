// Copyright 2024 GlitchyByte
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "gb.h"
#ifdef GB_TASK

#include "TaskRunner.h"
#include <atomic>
#include <mutex>
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

    public:
        uint64_t const taskId;

    private:
        std::thread thread;
        TaskRunner* runner { nullptr };
        TaskState state { TaskState::Created };
        std::mutex stateLock;
        std::condition_variable stateChangedSignal;
        std::atomic<bool> _shouldCancel { false };

    public:
        Task() noexcept : taskId(nextTaskId++) {};

        virtual ~Task() noexcept = default;

        virtual void action() = 0;

        void cancel() noexcept;

    protected:
        [[nodiscard]]
        TaskRunner* getTaskRunner() const noexcept;

        [[nodiscard]]
        bool shouldCancel() const noexcept;

    private:
        void setTaskRunner(TaskRunner* const newRunner) noexcept;

        void started() noexcept;

        void canceled() noexcept;

        void finished() noexcept;

        void awaitState(TaskState const desiredState) noexcept;

        void awaitStart() noexcept;

        void awaitStop() noexcept;
    };
}

#endif // GB_TASK
