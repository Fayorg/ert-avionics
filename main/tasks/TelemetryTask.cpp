// Created by Elie "Fayorg" Baier (elie.baier@epfl.ch) on 23/05/2025.
// Copyright (c) 2025 Elie Baier. All rights reserved.

#include "TelemetryTask.h"

#include "Communication.h"
#include "esp_log.h"
#include "PacketAssembler.h"
#include "PacketConstant.h"

void TelemetryTask::init() {
    ESP_LOGI("TelemetryTask", "Initializing TelemetryTask");

    ahrs.begin(20);

    bmp.init();
    mpu.init();
    hmc.init();

    ESP_LOGI("TelemetryTask", "Calibration started");
    bmp.calibrate();
    mpu.calibrate();
    hmc.calibrate();
    ESP_LOGI("TelemetryTask", "Calibration finished");
}

void TelemetryTask::run(void *args) {
    while (true) {
        esp_now_telemetry_payload_t telemetry_payload;

        telemetry_payload.pressure_hpa = bmp.get_pressure();
        telemetry_payload.temperature_c = bmp.get_temperature();
        telemetry_payload.altitude_m = bmp.get_altitude();

        telemetry_payload.accel_x = mpu.get_accel_x();
        telemetry_payload.accel_y = mpu.get_accel_y();
        telemetry_payload.accel_z = mpu.get_accel_z();

        float gx = mpu.get_gyro_x(), gy = mpu.get_gyro_y(), gz = mpu.get_gyro_z();
        float mx = hmc.get_mag_x(), my = hmc.get_mag_y(), mz = hmc.get_mag_z();

        hmc.display();

        // TODO: Switch axis if needed

        ahrs.update(gx, gy,  gz, telemetry_payload.accel_x, telemetry_payload.accel_y, telemetry_payload.accel_z, mx, my, mz);

        telemetry_payload.pitch = ahrs.getPitch();
        telemetry_payload.roll = ahrs.getRoll();
        telemetry_payload.yaw = ahrs.getYaw();

        telemetry_payload.flight_state = FlightState::getInstance().getState();
        telemetry_payload.parachute_unreefed = false;
        telemetry_payload.gliding = false;

        Communication::getInstance().send_packet(PacketAssembler::create_telemetry_packet(telemetry_payload), sizeof(telemetry_payload));

        vTaskDelay(pdMS_TO_TICKS(1000/20));
    }
}

UBaseType_t TelemetryTask::get_priority() const {
    return 5;
}

std::string TelemetryTask::getName() const {
    return "TelemetryTask";
}

std::vector<FlightState::State> TelemetryTask::shouldRunDuring() {
    return {FlightState::READY,
            FlightState::IN_FLIGHT,
            FlightState::GLIDING,
            FlightState::LANDED};
}




