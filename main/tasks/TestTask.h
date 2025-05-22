// Created by Elie "Fayorg" Baier (elie.baier@epfl.ch) on 22/05/2025.
// Copyright (c) 2025 Elie Baier. All rights reserved.

#ifndef TESTTASK_H
#define TESTTASK_H

#include "tasks/Task.h"

class TestTask : public Task {
    public:
        TestTask() = default;

        void run(void *args) override;
        std::vector<FlightState::State> shouldRunDuring() override;
        UBaseType_t get_priority() const override;
        void init() override;
        std::string getName() const override;
};



#endif //TESTTASK_H
