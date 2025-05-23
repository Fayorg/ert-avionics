// Created by Elie "Fayorg" Baier (elie.baier@epfl.ch) on 23/05/2025.
// Copyright (c) 2025 Elie Baier. All rights reserved.

#ifndef TELEMETRYTASK_H
#define TELEMETRYTASK_H

#include "BMP280.h"
#include "HMC5883L.h"
#include "MadgwickAHRS.h"
#include "MPU6050.h"
#include "Task.h"

class TelemetryTask : public Task {
    public:
        void init() override;
        void run(void *args) override;
        UBaseType_t get_priority() const override;
        std::vector<FlightState::State> shouldRunDuring() override;
        std::string getName() const override;

    private:
        MadgwickAHRS ahrs;
        BMP280 bmp;
        MPU6050 mpu;
        HMC5883L hmc;
};



#endif //TELEMETRYTASK_H
