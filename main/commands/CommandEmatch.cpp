// Created by Elie "Fayorg" Baier (elie.baier@epfl.ch) on 24/05/2025.
// Copyright (c) 2025 Elie Baier. All rights reserved.

#include "CommandEmatch.h"

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "Communication.h"
#include "PacketAssembler.h"
#include "PacketConstant.h"
#include "driver/gpio.h"

void CommandEmatch::execute(const std::vector<std::string> &args) {
    #ifdef IS_GROUND_STATION
        esp_now_command_payload_t payload = {
            .command_id = CMD_EMA,
            .command_data = 0,
        };
        Communication::getInstance().send_packet(PacketAssembler::create_command_packet(payload), sizeof(esp_now_command_payload_t));
    #else
        // Ematch command for avionics
        gpio_set_level(GPIO_NUM_4, 1);
        vTaskDelay(pdMS_TO_TICKS(5000));
        gpio_set_level(GPIO_NUM_4, 0);
    #endif
}

const std::string &CommandEmatch::name() const {
    return command_name;
}


