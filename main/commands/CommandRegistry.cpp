// Created by Elie "Fayorg" Baier (elie.baier@epfl.ch) on 21/05/2025.
// Copyright (c) 2025 Elie Baier. All rights reserved.

#include "CommandRegistry.h"

#include <sstream>

// This is giving me Java vibes :/
CommandRegistry& CommandRegistry::getInstance() {
    static CommandRegistry instance;
    return instance;
}

void CommandRegistry::registerCommand(std::shared_ptr<Command> command) {
    commands[command->name()] = command;
}

bool CommandRegistry::executeCommand(const std::string& input) {
    auto tokens = tokenize(input);
    if (tokens.empty()) return false;

    auto cmd = commands.find(tokens[0]);
    if (cmd != commands.end()) {
        tokens.erase(tokens.begin()); // remove command name
        cmd->second->execute(tokens);
        return true;
    }
    return false;
}

std::vector<std::string> CommandRegistry::tokenize(const std::string& str) {
    std::istringstream iss(str);
    std::vector<std::string> tokens;
    std::string s;
    while (iss >> s) tokens.push_back(s);
    return tokens;
}