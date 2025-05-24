// Created by Elie "Fayorg" Baier (elie.baier@epfl.ch) on 24/05/2025.
// Copyright (c) 2025 Elie Baier. All rights reserved.

#ifndef UNREEFEDTASK_H
#define UNREEFEDTASK_H
#include "Task.h"


class UnreefedTask : public Task {
    public:
        std::string getName() const override;
        void init() override;
        UBaseType_t get_priority() const override;
        void run(void *args) override;
        std::vector<FlightState::State> shouldRunDuring() override;
};



#endif //UNREEFEDTASK_H
