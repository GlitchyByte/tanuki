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

public:
    void action() override {
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
public:
    void action() override {
        started();
        std::this_thread::sleep_for(std::chrono::milliseconds(400));
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
