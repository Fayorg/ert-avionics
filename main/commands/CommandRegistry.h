// Created by Elie "Fayorg" Baier (elie.baier@epfl.ch) on 21/05/2025.
// Copyright (c) 2025 Elie Baier. All rights reserved.

#ifndef COMMANDREGISTRY_H
#define COMMANDREGISTRY_H

#include "Command.h"

#include <memory>
#include <map>

class CommandRegistry {
    public:
        static CommandRegistry& getInstance();

        void registerCommand(std::shared_ptr<Command> command);
        bool executeCommand(const std::string& input);

    private:
        std::map<std::string, std::shared_ptr<Command>> commands;
        std::vector<std::string> tokenize(const std::string& str);

        CommandRegistry() = default;
        CommandRegistry(const CommandRegistry&) = delete;
        CommandRegistry& operator=(const CommandRegistry&) = delete;
};

#endif //COMMANDREGISTRY_H
