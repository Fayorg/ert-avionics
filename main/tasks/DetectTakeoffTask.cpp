// Created by Elie "Fayorg" Baier (elie.baier@epfl.ch) on 23/05/2025.
// Copyright (c) 2025 Elie Baier. All rights reserved.

#include "DetectTakeoffTask.h"

#include "esp_log.h"
#include "Tools.h"

void DetectTakeoffTask::init() {}

UBaseType_t DetectTakeoffTask::get_priority() const {
    return 2;
}

std::string DetectTakeoffTask::getName() const {
    return "DetTakeoffTask";
}

void DetectTakeoffTask::run(void *args) {
    // std::vector<uint8_t> altitude_history;

    while (true) {
        if (!MPU6050::getInstance().read()) {
            ESP_LOGE("DetectTakeoffTask", "Failed to read MPU6050");
        }

        /*uint8_t altitude = bmp.get_altitude();
        altitude_history.push_back(altitude);
        if (altitude_history.size() > 50) {
            altitude_history.erase(altitude_history.begin());
        }*/

        float ax = MPU6050::getInstance().get_accel_x(), ay = MPU6050::getInstance().get_accel_y(), az = MPU6050::getInstance().get_accel_z();
        Tools::switchAxis(ax, ay, az);

        // Detecting the initial jerk
        if (az > 2.5) {
            // Checking if the altitude is increasing compare to the previous 50 values
            /*uint8_t history_min = *std::ranges::min_element(altitude_history);
            uint8_t history_max = *std::ranges::max_element(altitude_history);
            uint8_t history_avg = std::accumulate(altitude_history.begin(), altitude_history.end(), 0) / altitude_history.size();

            uint8_t delta = history_max - history_min;
            if (altitude > history_avg + delta) {
                ESP_LOGI("DetectTakeoffTask", "Takeoff detected with value %hhu (min: %hhu, max: %hhu, delta: %hhu)", altitude, history_min, history_max, delta);
            }*/
            ESP_LOGI("DetectTakeoffTask", "Takeoff detected with value %f", az);
            FlightState::getInstance().setState(FlightState::IN_FLIGHT);
        }

        vTaskDelay(pdMS_TO_TICKS(50));  // CPU-friendly delay
    }
}

std::vector<FlightState::State> DetectTakeoffTask::shouldRunDuring() {
    return {FlightState::READY, FlightState::ARMED};
}
