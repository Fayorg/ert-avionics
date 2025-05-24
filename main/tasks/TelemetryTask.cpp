// Created by Elie "Fayorg" Baier (elie.baier@epfl.ch) on 23/05/2025.
// Copyright (c) 2025 Elie Baier. All rights reserved.

#include "TelemetryTask.h"

#include "Communication.h"
#include "esp_log.h"
#include "PacketAssembler.h"
#include "PacketConstant.h"
#include "RedundantStorageManager.h"
#include "Tools.h"

void TelemetryTask::init() {
    ESP_LOGI("TelemetryTask", "Initializing TelemetryTask");

    ahrs.begin(20);

    hmc.init();

    ESP_LOGI("TelemetryTask", "Calibration started");
    hmc.calibrate();
    ESP_LOGI("TelemetryTask", "Calibration finished");

    RedundantStorageManager::getInstance().appendCsvData("/flight.csv", "========= INIT ========");
}

void TelemetryTask::run(void *args) {
    int count = 0;
    while (true) {
        esp_now_telemetry_payload_t telemetry_payload;

        if (!BMP280::getInstance().read()) {
            ESP_LOGE("TelemetryTask", "Failed to read BMP280");
        }
        if (!MPU6050::getInstance().read()) {
            ESP_LOGE("TelemetryTask", "Failed to read MPU6050");
        }
        if (!hmc.read()) {
            ESP_LOGE("TelemetryTask", "Failed to read HMC5883L");
        }

        telemetry_payload.pressure_hpa = BMP280::getInstance().get_pressure();
        telemetry_payload.temperature_c = BMP280::getInstance().get_temperature();
        telemetry_payload.altitude_m = BMP280::getInstance().get_altitude();

        float ax = MPU6050::getInstance().get_accel_x(), ay = MPU6050::getInstance().get_accel_y(), az = MPU6050::getInstance().get_accel_z();
        float gx = MPU6050::getInstance().get_gyro_x(), gy = MPU6050::getInstance().get_gyro_y(), gz = MPU6050::getInstance().get_gyro_z();
        float mx = hmc.get_mag_x(), my = hmc.get_mag_y(), mz = hmc.get_mag_z();

        // TODO: Switch axis if needed
        Tools::switchAxis(ax, ay, az); Tools::switchAxis(gx, gy, gz); Tools::switchAxis(mx, my, mz);

        telemetry_payload.accel_x = ax;
        telemetry_payload.accel_y = ay;
        telemetry_payload.accel_z = az;

        ahrs.update(gx, gy,  gz, telemetry_payload.accel_x, telemetry_payload.accel_y, telemetry_payload.accel_z, mx, my, mz);

        telemetry_payload.pitch = ahrs.getPitch();
        telemetry_payload.roll = ahrs.getRoll();
        telemetry_payload.yaw = ahrs.getYaw();

        telemetry_payload.flight_state = FlightState::getInstance().getState();
        telemetry_payload.parachute_unreefed = false;
        telemetry_payload.gliding = false;

        if (count % 4 == 0) {
            Communication::getInstance().send_packet(PacketAssembler::create_telemetry_packet(telemetry_payload), sizeof(telemetry_payload));
        }
        count++;

        RedundantStorageManager::getInstance().appendCsvData("/flight.csv",
            std::to_string(telemetry_payload.pressure_hpa) + "," +
            std::to_string(telemetry_payload.temperature_c) + "," +
            std::to_string(telemetry_payload.altitude_m) + "," +
            std::to_string(telemetry_payload.accel_x) + "," +
            std::to_string(telemetry_payload.accel_y) + "," +
            std::to_string(telemetry_payload.accel_z) + "," +
            std::to_string(telemetry_payload.pitch) + "," +
            std::to_string(telemetry_payload.roll) + "," +
            std::to_string(telemetry_payload.yaw));

        vTaskDelay(pdMS_TO_TICKS(1000/20));
    }
}

UBaseType_t TelemetryTask::get_priority() const {
    return 2;
}

std::string TelemetryTask::getName() const {
    return "TelemetryTask";
}

std::vector<FlightState::State> TelemetryTask::shouldRunDuring() {
    return {FlightState::ARMED,
            FlightState::IN_FLIGHT,
            FlightState::GLIDING,
            FlightState::LANDED,
            FlightState::UNREEFED
    };
}




