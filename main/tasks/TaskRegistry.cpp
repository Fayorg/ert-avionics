// Created by Elie "Fayorg" Baier (elie.baier@epfl.ch) on 21/05/2025.
// Copyright (c) 2025 Elie Baier. All rights reserved.

#include "TaskRegistry.h"

#include <sstream>

// This is giving me Java vibes :/
TaskRegistry& TaskRegistry::getInstance() {
    static TaskRegistry instance;
    return instance;
}

void TaskRegistry::registerTask(std::shared_ptr<Task> task) {
    tasks.push_back(task);
}

void TaskRegistry::initTasks() {
    for (const auto& task : tasks) {
        task->init();

        // if (taskShouldRun())
    }
}

bool TaskRegistry::taskShouldRun(FlightState::State current_state, const std::shared_ptr<Task> &task) {
    if (task->shouldRunDuring().empty()) {
        return true;
    }

    return task->shouldRunDuring().empty() || std::find(task->shouldRunDuring().begin(), task->shouldRunDuring().end(), current_state) != task->shouldRunDuring().end();
}
