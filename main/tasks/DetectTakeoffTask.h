// Created by Elie "Fayorg" Baier (elie.baier@epfl.ch) on 23/05/2025.
// Copyright (c) 2025 Elie Baier. All rights reserved.

#ifndef DETECTTAKEOFFTASK_H
#define DETECTTAKEOFFTASK_H

#include <tasks/Task.h>

#include "BMP280.h"
#include "HMC5883L.h"
#include "MPU6050.h"

class DetectTakeoffTask : public Task {
    public:
        void init() override;
        UBaseType_t get_priority() const override;
        std::string getName() const override;
        void run(void *args) override;
        std::vector<FlightState::State> shouldRunDuring() override;

    private:
        BMP280 bmp;
        MPU6050 mpu;
        HMC5883L hmc;
};



#endif //DETECTTAKEOFFTASK_H
