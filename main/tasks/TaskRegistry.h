// Created by Elie "Fayorg" Baier (elie.baier@epfl.ch) on 21/05/2025.
// Copyright (c) 2025 Elie Baier. All rights reserved.

#ifndef TASKREGISTRY_H
#define TASKREGISTRY_H

#include "Task.h"

#include <memory>
#include <map>

class TaskRegistry {
    public:
        static TaskRegistry& getInstance();

        void registerTask(const std::shared_ptr<Task>& command);

        void initTasks();
        void onStateChange(FlightState::State oldState, FlightState::State newState);

        std::vector<std::shared_ptr<Task>> getTasks() {
            return tasks;
        }

        std::map<std::shared_ptr<Task>, std::shared_ptr<TaskHandle_t>> getRunningTasks() {
            return runningTasks;
        }

    private:
        std::map<std::shared_ptr<Task>, std::shared_ptr<TaskHandle_t>> runningTasks;
        std::vector<std::shared_ptr<Task>> tasks;

        static bool taskShouldRun(FlightState::State current_state, const std::shared_ptr<Task>& task);

        TaskRegistry() = default;
        TaskRegistry(const TaskRegistry&) = delete;
        TaskRegistry& operator=(const TaskRegistry&) = delete;
};

#endif //TASKREGISTRY_H
