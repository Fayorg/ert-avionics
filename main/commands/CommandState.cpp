// Created by Elie "Fayorg" Baier (elie.baier@epfl.ch) on 23/05/2025.
// Copyright (c) 2025 Elie Baier. All rights reserved.

#include "CommandState.h"

#include "Communication.h"
#include "FlightState.h"
#include "PacketAssembler.h"

void CommandState::execute(const std::vector<std::string> &args) {
    #ifdef IS_GROUND_STATION
        if (args.size() < 1) return;
        // get first arg and cast to int
        uint8_t flight_state = std::stoi(args[0]);
        esp_now_command_payload_t payload = {
            .command_id = CMD_SET_STATE,
            .command_data = flight_state
        };
        Communication::getInstance().send_packet(PacketAssembler::create_command_packet(payload), sizeof(esp_now_command_payload_t));
    #else
        if (args.size() < 1) return;

        if (args[0] == "SET") {
            if (args.size() < 2) return;
            uint8_t flight_state = std::stoi(args[1]);

            FlightState::getInstance().setState(static_cast<FlightState::State>(flight_state));
            ESP_LOGI("CommandState", "Flight state set to %d", flight_state);
        } else if (args[0] == "GET") {
            ESP_LOGI("CommandState", "Current flight state: %d", FlightState::getInstance().getState());
        } else {
            ESP_LOGE("CommandState", "Unknown command: %s", args[0].c_str());
        }
    #endif
}

const std::string &CommandState::name() const {
    return command_name;
}

