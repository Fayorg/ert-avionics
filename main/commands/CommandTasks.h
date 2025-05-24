// Created by Elie "Fayorg" Baier (elie.baier@epfl.ch) on 23/05/2025.
// Copyright (c) 2025 Elie Baier. All rights reserved.

#ifndef COMMANDTASKS_H
#define COMMANDTASKS_H
#include "Command.h"


class CommandTasks : public Command {
    public:
        const std::string& name() const override;
        void execute(const std::vector<std::string>& args) override;
    private:
        std::string cmd_name = "tasks";
};



#endif //COMMANDTASKS_H
