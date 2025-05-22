// Created by Elie "Fayorg" Baier (elie.baier@epfl.ch) on 22/05/2025.
// Copyright (c) 2025 Elie Baier. All rights reserved.

#ifndef TASK_H
#define TASK_H

#include <portmacro.h>
#include <vector>
#include <state/FlightState.h>

class Task {
    public:
        Task() = default;
        virtual ~Task() = default;

        virtual void run() = 0;
        virtual void init() = 0;

        virtual UBaseType_t get_priority() const = 0;

        // This should return a vector of FlightState::State that the task should run during. If nothing is provided, the task will run during all states.
        virtual std::vector<FlightState::State> shouldRunDuring() = 0;
};

#endif //TASK_H
