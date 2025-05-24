// Created by Elie "Fayorg" Baier (elie.baier@epfl.ch) on 24/05/2025.
// Copyright (c) 2025 Elie Baier. All rights reserved.

#include "CommandRs.h"

#include "Communication.h"
#include "esp_system.h"
#include "PacketAssembler.h"

void CommandRs::execute(const std::vector<std::string> &args) {
    if (args.size() < 1) return;
    if (args[0] == "GND") {
        esp_restart();
    } else if (args[0] == "AVI") {
        if (args.size() < 2) {
            esp_now_command_payload_t payload = {
                .command_id = CMD_RS,
                .command_data = 0,
            };
            Communication::getInstance().send_packet(PacketAssembler::create_command_packet(payload), sizeof(payload));
        } else {
            esp_now_command_payload_t payload = {
                .command_id = CMD_RS,
                .command_data = 1,
            };
            Communication::getInstance().send_packet(PacketAssembler::create_command_packet(payload), sizeof(payload));
        }
    }
}

const std::string &CommandRs::name() const {
    return command_name;
}

