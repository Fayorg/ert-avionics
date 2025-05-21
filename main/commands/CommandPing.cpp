// Created by Elie "Fayorg" Baier (elie.baier@epfl.ch) on 21/05/2025.
// Copyright (c) 2025 Elie Baier. All rights reserved.

#include "CommandPing.h"

const std::string& CommandPing::name() const {
    return cmd_name;
}

void CommandPing::execute(const std::vector<std::string>& args) {
    if (args.empty()) {
        printf("Pong!\n");
    } else {
        printf("Pong! Args: ");
        for (const auto& arg : args) {
            printf("%s ", arg.c_str());
        }
        printf("\n");
    }
}