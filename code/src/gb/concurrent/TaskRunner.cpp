// Copyright 2024 GlitchyByte
// SPDX-License-Identifier: Apache-2.0

#include "gb/concurrent/TaskRunner.h"
#ifdef GB_TASK

namespace gb::concurrent {

    bool TaskRunner::taskComparator(std::shared_ptr<Task> const& lhs, std::shared_ptr<Task> const& rhs) noexcept {
        return lhs->taskId < rhs->taskId;
    }

    TaskRunner::TaskRunner() noexcept {
        threadController = std::thread {
            // Waits for threads to join and removes them from the set.
            [this]{
                std::unique_lock<std::mutex> lock { threadControllerLock };
                while (true) {
                    threadControllerWakeUpSignal.wait(lock, [this]{ return threadControllerShouldExit || !finishingTasks.empty(); });
                    if (threadControllerShouldExit) {
                        break;
                    }
                    for (auto& task: finishingTasks) {
                        task->thread.join();
                        removeTask(task);
                    }
                    finishingTasks.clear();
                }
            }
        };
    }

    TaskRunner::~TaskRunner() noexcept {
        shutdown();
    }

    bool TaskRunner::isActive() const noexcept {
        return _isActive.load();
    }

    void TaskRunner::shutdown() noexcept {
        std::unique_lock<std::mutex> lock { tasksLock };
        if (!_isActive.compare_exchange_strong(_true, false)) {
            return;
        }
        for (auto const& task: tasks) {
            task->cancel();
        }
        isEmptySignal.wait(lock, [this]{ return tasks.empty(); });
        lock.unlock();
        std::unique_lock<std::mutex> controllerLock { threadControllerLock };
        threadControllerShouldExit = true;
        threadControllerWakeUpSignal.notify_one();
        controllerLock.unlock();
        threadController.join();
    }

//    template<class TTask>
//    requires std::is_base_of<Task, TTask>::value
//    bool TaskRunner::start(std::shared_ptr<TTask> const& task) noexcept {
    bool TaskRunner::start(std::shared_ptr<Task> const& task) noexcept {
        std::lock_guard<std::mutex> lock { tasksLock };
        if (!isActive()) {
            return false;
        }
        auto const [_, success] = tasks.insert(task);
        if (!success) {
            return false;
        }
        task->setTaskRunner(this);
        task->thread = std::thread {
            [this, task]{
                task->action();
                task->finished();
                std::lock_guard<std::mutex> lock { threadControllerLock };
                finishingTasks.push_back(task);
                threadControllerWakeUpSignal.notify_one();
            }
        };
        task->awaitStart();
        return true;
    }

    void TaskRunner::cancelAll() noexcept {
        std::lock_guard<std::mutex> lock { tasksLock };
        for (auto const& task: tasks) {
            task->cancel();
        }
    }

    void TaskRunner::removeTask(std::shared_ptr<Task> const& task) noexcept {
        std::lock_guard<std::mutex> lock { tasksLock };
        tasks.erase(task);
        if (tasks.empty()) {
            isEmptySignal.notify_one();
        }
    }
}

#endif // GB_TASK
