// Created by Elie "Fayorg" Baier (elie.baier@epfl.ch) on 24/05/2025.
// Copyright (c) 2025 Elie Baier. All rights reserved.

#include "ApogeeTask.h"

#include "esp_log.h"

void ApogeeTask::init() {}

std::string ApogeeTask::getName() const {
    return "ApogeeTask";
}

UBaseType_t ApogeeTask::get_priority() const {
    return 5;
}

void ApogeeTask::run(void *args) {
    ESP_LOGI("ApogeeTask", "Staring ApogeeTask");

    while (true) {
        if (BMP280::getInstance().read()) {
            ESP_LOGE("ApogeeTask", "Failed to read BMP280");
        }

        if (BMP280::getInstance().get_max_altitude_reached()) {
            FlightState::getInstance().setState(FlightState::GLIDING);
        }

        vTaskDelay(pdMS_TO_TICKS(50));
    }
}

std::vector<FlightState::State> ApogeeTask::shouldRunDuring() {
    return {FlightState::IN_FLIGHT};
}
