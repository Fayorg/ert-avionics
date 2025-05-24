// Created by Elie "Fayorg" Baier (elie.baier@epfl.ch) on 24/05/2025.
// Copyright (c) 2025 Elie Baier. All rights reserved.

#ifndef DETECTAPOGEETASK_H
#define DETECTAPOGEETASK_H
#include "Task.h"


class DetectApogeeTask : public Task {
    public:
        void init() override;
        UBaseType_t get_priority() const override;
        std::string getName() const override;
        void run(void *args) override;
        std::vector<FlightState::State> shouldRunDuring() override;
};



#endif //DETECTAPOGEETASK_H
