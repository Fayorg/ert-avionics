// Created by Elie "Fayorg" Baier (elie.baier@epfl.ch) on 23/05/2025.
// Copyright (c) 2025 Elie Baier. All rights reserved.

#ifndef STATECOMMAND_H
#define STATECOMMAND_H
#include "Command.h"


class CommandState : public Command {
    public:
        void execute(const std::vector<std::string> &args) override;
        const std::string &name() const override;

    private:
        const std::string command_name = "state";
};



#endif //STATECOMMAND_H
