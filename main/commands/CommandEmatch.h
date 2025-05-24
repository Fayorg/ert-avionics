// Created by Elie "Fayorg" Baier (elie.baier@epfl.ch) on 24/05/2025.
// Copyright (c) 2025 Elie Baier. All rights reserved.

#ifndef COMMANDEMATCH_H
#define COMMANDEMATCH_H
#include "Command.h"


class CommandEmatch : public Command {
    public:
        void execute(const std::vector<std::string> &args) override;
        const std::string &name() const override;

    private:
        const std::string command_name = "ema";
};



#endif //COMMANDEMATCH_H
