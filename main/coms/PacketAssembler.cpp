// Created by Elie "Fayorg" Baier (elie.baier@epfl.ch) on 22/05/2025.
// Copyright (c) 2025 Elie Baier. All rights reserved.

#include "PacketAssembler.h"

#include <Communication.h>

esp_now_telemetry_packet_t PacketAssembler::create_telemetry_packet() {
    esp_now_packet_header_t header = {
        .packet_type = PACKET_TYPE_TELEMETRY,
        .sequence_num = 0, // TODO
        .sender_mac = *Communication::getInstance().get_mac_addr(),
        .reserved = 0
    };


}

esp_now_heartbeat_packet_t PacketAssembler::create_heartbeat_packet() {
    auto header = create_packet_header(PACKET_TYPE_HEARTBEAT);

    esp_now_heartbeat_payload_t payload = {
        .flight_state = 0,
    };

    return {
        .header = header,
        .heartbeat_payload = payload,
        .crc = 0 // TODO: Calculate CRC
    };
}

esp_now_packet_header_t PacketAssembler::create_packet_header(uint8_t packet_type) {
    return {
        .packet_type = packet_type,
        .sequence_num = Communication::getInstance().get_sequence_num(),
        .sender_mac = *Communication::getInstance().get_mac_addr(),
        .reserved = 0
    };
}


