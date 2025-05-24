// Created by Elie "Fayorg" Baier (elie.baier@epfl.ch) on 24/05/2025.
// Copyright (c) 2025 Elie Baier. All rights reserved.

#include "UnreefedTask.h"

#include "BMP280.h"
#include "esp_log.h"

void UnreefedTask::init() {}

void UnreefedTask::run(void *args) {
    ESP_LOGI("UnreefedTask", "Staring ApogeeTask");

    while (true) {
        if (BMP280::getInstance().read()) {
            ESP_LOGE("UnreefedTask", "Failed to read BMP280");
        }

        if (BMP280::getInstance().get_deploy_main_para_parachute()) {
            gpio_set_level(GPIO_NUM_4, 1);
            vTaskDelay(pdMS_TO_TICKS(5000));
            gpio_set_level(GPIO_NUM_4, 0);

            FlightState::getInstance().setState(FlightState::UNREEFED);
        }

        vTaskDelay(pdMS_TO_TICKS(50));
    }
}

UBaseType_t UnreefedTask::get_priority() const {
    return 5;
}

std::string UnreefedTask::getName() const {
    return "UnreefedTask";
}

std::vector<FlightState::State> UnreefedTask::shouldRunDuring() {
    return {FlightState::GLIDING};
}



