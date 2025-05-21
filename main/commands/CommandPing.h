// Created by Elie "Fayorg" Baier (elie.baier@epfl.ch) on 21/05/2025.
// Copyright (c) 2025 Elie Baier. All rights reserved.

#ifndef COMMANDPING_H
#define COMMANDPING_H

#include "Command.h"

class CommandPing : public Command {
    public:
        const std::string& name() const override;
        void execute(const std::vector<std::string>& args) override;
    private:
        std::string cmd_name = "ping";
};

#endif //COMMANDPING_H
