// Created by Elie "Fayorg" Baier (elie.baier@epfl.ch) on 22/05/2025.
// Copyright (c) 2025 Elie Baier. All rights reserved.

#ifndef HEARTBEATTASK_H
#define HEARTBEATTASK_H

#include "tasks/Task.h"
#include "state/FlightState.h"

class HeartbeatTask : public Task {
public:
    HeartbeatTask() = default;

    void run(void* args) override;
    void init() override;

    UBaseType_t get_priority() const override {
        return 3;
    }

    std::vector<FlightState::State> shouldRunDuring() override {
        return {};
    }

    std::string getName() const override;
};



#endif //HEARTBEATTASK_H
