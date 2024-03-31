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

    /**
     * Task states.
     */
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
        /**
         * Lock for any state related operation.
         */
        std::mutex stateLock;

    private:
        uint64_t const taskId;
        std::thread thread;
        TaskRunner* runner { nullptr };
        std::atomic<TaskState> state { TaskState::Created };
        std::condition_variable stateChangedSignal;
        std::atomic<bool> _shouldCancel { false };

    public:
        /**
         * Creates a task with a unique id.
         */
        Task() noexcept : taskId(nextTaskId++) {};

        virtual ~Task() noexcept = default;

        /**
         * Returns the current state of the task, as of the time of calling this method.
         *
         * <p>Note that the state can change by the time you are comparing the result.
         *
         * @return The current state of the task.
         */
        TaskState getState() const noexcept;

        /**
         * Signals the task to cancel.
         *
         * <p>It's up to the task itself to check for its cancellation and needing to exit.
         */
        void cancel() noexcept;

        /**
         * Blocks the calling thread until the task stops, either by cancellation or by finishing.
         */
        void awaitStop() noexcept;

        /**
         * Tests if the task has stopped, either by cancellation or by finishing.
         *
         * <p>Once the task enters a stopped state its state will never change again.
         *
         * @return True if the task has stopped.
         */
        bool isStopped() const noexcept;

    protected:
        /**
         * Actual action that is executed on its own thread.
         *
         * <p>Implementations MUST call started(), usually after initialization, indicating the task is ready and running.
         */
        virtual void action() noexcept = 0;

        /**
         * Returns the task runner that is running this task.
         *
         * <p>Can be used to start other tasks with the same runner.
         *
         * @return The task runner that is running this task.
         */
        [[nodiscard]]
        TaskRunner* getTaskRunner() const noexcept;

        /**
         * Signals the tasks internals that the action has started and is functional.
         *
         * <p>It MUST be called within action by implementations.
         */
        void started() noexcept;

        /**
         * Tests if the task was canceled and should exit.
         *
         * <p>Long running action implementations should check this periodically and exit when requested.
         *
         * @return True if the task was canceled and should exit.
         */
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
