// Created by Elie "Fayorg" Baier (elie.baier@epfl.ch) on 23/05/2025.
// Copyright (c) 2025 Elie Baier. All rights reserved.

#ifndef FORWARDUARTTASK_H
#define FORWARDUARTTASK_H
#include "Task.h"


class ForwardUartTask : public Task {
    public:
        void init() override;
        UBaseType_t get_priority() const override;
        std::string getName() const override;
        void run(void *args) override;
        std::vector<FlightState::State> shouldRunDuring() override;
};



#endif //FORWARDUARTTASK_H
