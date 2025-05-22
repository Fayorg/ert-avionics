// Created by Elie "Fayorg" Baier (elie.baier@epfl.ch) on 21/05/2025.
// Copyright (c) 2025 Elie Baier. All rights reserved.

#ifndef COMMANDREGISTRY_H
#define COMMANDREGISTRY_H

#include "Task.h"

#include <memory>
#include <map>
#include <freertos/task.h>

class TaskRegistry {
    public:
        static TaskRegistry& getInstance();

        void registerTask(std::shared_ptr<Task> command);

        void initTasks();
        void onStateChange(FlightState::State oldState, FlightState::State newState);

    private:
        std::map<std::shared_ptr<Task>, TaskHandle_t> runningTasks;
        std::vector<std::shared_ptr<Task>> tasks;

    bool taskShouldRun(FlightState::State current_state, const std::shared_ptr<Task>& task);

        TaskRegistry() = default;
        TaskRegistry(const TaskRegistry&) = delete;
        TaskRegistry& operator=(const TaskRegistry&) = delete;
};

#endif //COMMANDREGISTRY_H
