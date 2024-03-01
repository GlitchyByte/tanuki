// Copyright 2024 GlitchyByte
// SPDX-License-Identifier: Apache-2.0

#include "gb.h"
#include <gtest/gtest.h>

class TaskRunnerTest : public ::testing::Test {
protected:
    gb::concurrent::TaskRunner* runner { nullptr };

protected:
    void SetUp() override {
        runner = new gb::concurrent::TaskRunner();
    }

    void TearDown() override {
        delete runner;
    }
};

class SimpleTask : public gb::concurrent::Task {
public:
    std::set<std::string> items;
    std::mutex itemsLock;

protected:
    void action() noexcept override {
        started();
        addItem("one");
    }

    void addItem(std::string const& item) {
        std::lock_guard<std::mutex> lock { itemsLock };
        items.insert(item);
    }
};

TEST_F(TaskRunnerTest, CanStartTask) {
    std::shared_ptr<SimpleTask> task = std::make_shared<SimpleTask>();
    ASSERT_TRUE(task->items.empty());
    runner->start(task);
    task->awaitStop();
    ASSERT_TRUE(task->items.contains("one"));
}

class SlowTask : public SimpleTask {
protected:
    void action() noexcept override {
        started();
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
        if (shouldCancel()) {
            return;
        }
        addItem("one");
    }
};

TEST_F(TaskRunnerTest, CanCancelTask) {
    std::shared_ptr<SlowTask> task = std::make_shared<SlowTask>();
    ASSERT_TRUE(task->items.empty());
    runner->start(task);
    task->cancel();
    task->awaitStop();
    ASSERT_FALSE(task->items.contains("one"));
}

class CancelableTask : public gb::concurrent::Task {
public:
    static std::set<std::string> items;
    static std::mutex itemsLock;

protected:
    static void addItem(std::string const& item) {
        std::lock_guard<std::mutex> lock { itemsLock };
        items.insert(item);
    }

private:
    std::string const itemToAdd;

public:
    explicit CancelableTask(std::string const& str) noexcept : itemToAdd(str) {};

protected:
    void action() noexcept override {
        started();
        while (!shouldCancel()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        addItem(itemToAdd);
    }
};

std::set<std::string> CancelableTask::items;
std::mutex CancelableTask::itemsLock;

TEST_F(TaskRunnerTest, CanCancelAllTasks) {
    std::shared_ptr<CancelableTask> task1 = std::make_shared<CancelableTask>("one");
    std::shared_ptr<CancelableTask> task2 = std::make_shared<CancelableTask>("two");
    std::shared_ptr<CancelableTask> task3 = std::make_shared<CancelableTask>("three");
    ASSERT_TRUE(CancelableTask::items.empty());
    runner->start(task1);
    runner->start(task2);
    runner->start(task3);
    runner->cancelAll();
    runner->awaitAll();
    ASSERT_TRUE(CancelableTask::items.contains("one"));
    ASSERT_TRUE(CancelableTask::items.contains("two"));
    ASSERT_TRUE(CancelableTask::items.contains("three"));
}
