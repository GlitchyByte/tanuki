// Copyright 2024 GlitchyByte
// SPDX-License-Identifier: Apache-2.0

#include "gb/concurrent/Task.h"
#ifdef GB_TASK

namespace gb::concurrent {

    std::atomic<uint64_t> Task::nextTaskId { 0 };

    void Task::cancel() noexcept {
        std::lock_guard<std::mutex> lock { stateLock };
        if (state != TaskState::Started) {
            return;
        }
        _shouldCancel = true;
    }

    void Task::awaitStop() noexcept {
        std::unique_lock<std::mutex> lock { stateLock };
        stateChangedSignal.wait(lock, [this]{ return (state == TaskState::Canceled) || (state == TaskState::Finished); });
    }

    TaskRunner* Task::getTaskRunner() const noexcept {
        return runner;
    }

    void Task::started() noexcept {
        std::lock_guard<std::mutex> lock { stateLock };
        if (state != TaskState::Created) {
            return;
        }
        state = TaskState::Started;
        stateChangedSignal.notify_all();
    }

    bool Task::shouldCancel() const noexcept {
        return _shouldCancel;
    }

    void Task::setTaskRunner(TaskRunner* const newRunner) noexcept {
        runner = newRunner;
    }

    void Task::canceled() noexcept {
        std::lock_guard<std::mutex> lock { stateLock };
        if (state != TaskState::Started) {
            return;
        }
        state = TaskState::Canceled;
        stateChangedSignal.notify_all();
    }

    void Task::finished() noexcept {
        std::lock_guard<std::mutex> lock { stateLock };
        if (state != TaskState::Started) {
            return;
        }
        state = TaskState::Finished;
        stateChangedSignal.notify_all();
    }

    void Task::awaitState(TaskState const desiredState) noexcept {
        std::unique_lock<std::mutex> lock { stateLock };
        stateChangedSignal.wait(lock, [this, desiredState]{ return state == desiredState; });
    }

    void Task::awaitStart() noexcept {
        std::unique_lock<std::mutex> lock { stateLock };
        stateChangedSignal.wait(lock, [this]{ return state != TaskState::Created; });
    }
}

#endif // GB_TASK
