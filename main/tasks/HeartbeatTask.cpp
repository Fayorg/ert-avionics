// Created by Elie "Fayorg" Baier (elie.baier@epfl.ch) on 22/05/2025.
// Copyright (c) 2025 Elie Baier. All rights reserved.

#include "HeartbeatTask.h"

#include "Communication.h"
#include "esp_log.h"
#include "PacketAssembler.h"

void HeartbeatTask::init() {}

void HeartbeatTask::run(void* args) {
    while (true) {
        auto packet = PacketAssembler::create_heartbeat_packet();
        Communication::getInstance().send_packet(packet, sizeof(packet.heartbeat_payload));

        vTaskDelay(pdMS_TO_TICKS(5000));  // CPU-friendly delay
    }
}

std::string HeartbeatTask::getName() const {
    return "HeartbeatTask";
}


