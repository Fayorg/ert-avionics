// Created by Elie "Fayorg" Baier (elie.baier@epfl.ch) on 24/05/2025.
// Copyright (c) 2025 Elie Baier. All rights reserved.

#ifndef COMMANDRS_H
#define COMMANDRS_H
#include "Command.h"


class CommandRs : public Command {
    public:
        void execute(const std::vector<std::string> &args) override;
        const std::string &name() const override;

    private:
        std::string command_name = "rs";
};



#endif //COMMANDRS_H
