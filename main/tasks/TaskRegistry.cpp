// Created by Elie "Fayorg" Baier (elie.baier@epfl.ch) on 21/05/2025.
// Copyright (c) 2025 Elie Baier. All rights reserved.

#include "TaskRegistry.h"

#include <esp_log.h>
#include <sstream>
#include <freertos/FreeRTOSConfig.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

// This is giving me Java vibes :/
TaskRegistry& TaskRegistry::getInstance() {
    static TaskRegistry instance;
    return instance;
}

void TaskRegistry::registerTask(const std::shared_ptr<Task>& task) {
    tasks.push_back(task);
}

void TaskRegistry::initTasks() {
    ESP_LOGI("TaskRegistry", "Task registration started");
    for (const auto& task : tasks) {
        task->init();

        if (taskShouldRun(FlightState::getInstance().getState(), task)) {
            TaskHandle_t taskHandle;
            auto status = xTaskCreate(Task::taskRunner, task->getName().c_str(), 4096, task.get(), task->get_priority(), &taskHandle);
            if (status != pdPASS) {
                ESP_LOGE("TaskRegistry", "Failed to create task: %s", task->getName().c_str());
            } else {
                runningTasks[task->getName()] = std::make_shared<TaskHandle_t>(taskHandle);
                ESP_LOGI("TaskRegistry", "Created task: %s", task->getName().c_str());
            }
        }
    }
}

void TaskRegistry::onStateChange(FlightState::State oldState, FlightState::State newState) {
    for (auto& task : tasks) {
        if (taskShouldRun(newState, task)) {
            auto it = runningTasks.find(task->getName());
            if (it == runningTasks.end()) {
                TaskHandle_t taskHandle;
                auto status = xTaskCreate(Task::taskRunner, task->getName().c_str(), 4096, task.get(), task->get_priority(), &taskHandle);

                if (status != pdPASS) {
                    ESP_LOGE("TaskRegistry", "Failed to create task: %s", task->getName().c_str());
                } else {
                    runningTasks[task->getName()] = std::make_shared<TaskHandle_t>(taskHandle);
                    ESP_LOGI("TaskRegistry", "Created task: %s", task->getName().c_str());
                }
            }
        } else {
            auto it = runningTasks.find(task->getName());
            if (it != runningTasks.end()) {
                vTaskDelete(*it->second);
                runningTasks.erase(it);
                ESP_LOGI("TaskRegistry", "Deleted task: %s", task->getName().c_str());
            } else {
                ESP_LOGI("TaskRegistry", "Task %s is not running", task->getName().c_str());
            }
        }
    }
}


bool TaskRegistry::taskShouldRun(const FlightState::State current_state, const std::shared_ptr<Task> &task) {
    if (task->shouldRunDuring().empty()) {
        return true;
    }

    auto states = task->shouldRunDuring();
    auto it = std::find(states.begin(), states.end(), current_state);

    return it != states.end();
}
