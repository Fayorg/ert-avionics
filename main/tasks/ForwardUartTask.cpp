// Created by Elie "Fayorg" Baier (elie.baier@epfl.ch) on 23/05/2025.
// Copyright (c) 2025 Elie Baier. All rights reserved.

#include "ForwardUartTask.h"

void ForwardUartTask::init() {

}

void ForwardUartTask::run(void *args) {

}

UBaseType_t ForwardUartTask::get_priority() const {
    return 5;
}

std::string ForwardUartTask::getName() const {
    return "ForwardUartTask";
}

std::vector<FlightState::State> ForwardUartTask::shouldRunDuring() {
    return {FlightState::State::INIT, FlightState::READY};
}