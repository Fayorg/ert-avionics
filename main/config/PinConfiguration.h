// Created by Elie "Fayorg" Baier (elie.baier@epfl.ch) on 21/05/2025.
// Copyright (c) 2025 Elie Baier. All rights reserved.

#ifndef PINCONFIGURATION_H
#define PINCONFIGURATION_H

#define SDSPI_HOST_ID   SPI2_HOST

#define PIN_NUM_MISO    GPIO_NUM_19
#define PIN_NUM_MOSI    GPIO_NUM_23
#define PIN_NUM_CLK     GPIO_NUM_18
#define PIN_NUM_CS      GPIO_NUM_5

constexpr double MAIN_PARA_DEPLOY_HEIGHT = 150; // meters //height where it deploys
constexpr double MAIN_PARA_DEPLOY_HEIGHT_TOLERANCE = 4; // distance [m] the baro needs to drop before max height is declared to be reached
constexpr double MAIN_PARA_DEPLOY_HEIGHT_WAIT =  1; // seconds (height to wait before deploying the main parachute if it hasn't reached 150m)
constexpr double SAMPLE_FREQ = 20; // Hz | t[ms]= 1000/SAMPLE_FREQ
constexpr double MAIN_PARA_DEPLOY_VELOCITY = -1; // [m/s]

#endif //PINCONFIGURATION_H
