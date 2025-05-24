// Created by Elie "Fayorg" Baier (elie.baier@epfl.ch) on 23/05/2025.
// Copyright (c) 2025 Elie Baier. All rights reserved.

#include "CommandTasks.h"

#include "esp_log.h"
#include "TaskRegistry.h"

void CommandTasks::execute(const std::vector<std::string> &args) {
    printf("List of tasks:\n");

    auto running = TaskRegistry::getInstance().getRunningTasks();
    for (auto task : TaskRegistry::getInstance().getTasks()) {
        printf("%s", task->getName().c_str());
        if (running.contains(task->getName())) {
            printf(" (running)\n");
        } else {
            printf("\n");
        }
    }
}

const std::string &CommandTasks::name() const {
    return cmd_name;
}

