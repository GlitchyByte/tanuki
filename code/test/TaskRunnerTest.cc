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

class ValueSetterTask : public gb::concurrent::Task {
public:
    std::atomic<int> value { 1 };

public:
    void action() override {
        started();
        value = 5;
    }
};

TEST_F(TaskRunnerTest, RunnerStartsTask) {
    std::shared_ptr<ValueSetterTask> task = std::make_shared<ValueSetterTask>();
    ASSERT_EQ(1, task->value);
    runner->start(task);
    task->awaitStop();
    ASSERT_EQ(5, task->value);
}

TEST_F(TaskRunnerTest, Dummy) {
    std::shared_ptr<ValueSetterTask> task = std::make_shared<ValueSetterTask>();
    ASSERT_EQ(1, task->value);
    runner->start(task);
    task->awaitStop();
    ASSERT_NE(4, task->value);
}
