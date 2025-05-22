// Created by Elie "Fayorg" Baier (elie.baier@epfl.ch) on 22/05/2025.
// Copyright (c) 2025 Elie Baier. All rights reserved.

#include "TestTask.h"

#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

void TestTask::init() {
    ESP_LOGI("TestTask", "Initializing TestTask");
}

void TestTask::run(void* args) {
    while (true) {
        ESP_LOGI("TestTask", "Running TestTask");
        vTaskDelay(pdMS_TO_TICKS(1000)); // Sleep for 1 second
    }
}

UBaseType_t TestTask::get_priority() const {
    return 5;
}

std::vector<FlightState::State> TestTask::shouldRunDuring() {
    return {};
}

std::string TestTask::getName() const {
    return "TestTask";
}




