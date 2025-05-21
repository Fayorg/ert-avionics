// Created by Elie "Fayorg" Baier (elie.baier@epfl.ch) on 21/05/2025.
// Copyright (c) 2025 Elie Baier. All rights reserved.

#ifndef COMMAND_H
#define COMMAND_H

#include <string>
#include <vector>

class Command {
    public:
        virtual ~Command() = default;
        [[nodiscard]] virtual const std::string& name() const = 0;
        virtual void execute(const std::vector<std::string>& args) = 0;
};

#endif //COMMAND_H
