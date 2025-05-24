// Created by Elie "Fayorg" Baier (elie.baier@epfl.ch) on 22/05/2025.
// Copyright (c) 2025 Elie Baier. All rights reserved.

#include "PacketAssembler.h"

#include <Communication.h>

#include "FlightState.h"

esp_now_telemetry_packet_t PacketAssembler::create_telemetry_packet(esp_now_telemetry_payload_t payload) {
    auto header = create_packet_header(PACKET_TYPE_TELEMETRY);

    return {
        .header = header,
        .telemetry_payload = payload,
        .crc = 0 // TODO: Calculate CRC
    };
}

esp_now_heartbeat_packet_t PacketAssembler::create_heartbeat_packet() {
    auto header = create_packet_header(PACKET_TYPE_HEARTBEAT);

    esp_now_heartbeat_payload_t payload = {
        .flight_state = static_cast<uint8_t>(FlightState::getInstance().getState()),
    };

    return {
        .header = header,
        .heartbeat_payload = payload,
        .crc = 0 // TODO: Calculate CRC
    };
}

esp_now_ack_packet_t PacketAssembler::create_ack_packet(esp_now_ack_status_t status, uint8_t sequence) {
    auto header = create_packet_header(PACKET_TYPE_ACK);

    esp_now_ack_payload_t payload = {
        .ack_status = status,
        .acked_sequence_num = sequence,
    };

    return {
        .header = header,
        .ack_payload = payload,
        .crc = 0 // TODO: Calculate CRC
    };
}

esp_now_command_packet_t PacketAssembler::create_command_packet(esp_now_command_payload_t payload) {
    auto header = create_packet_header(PACKET_TYPE_COMMAND);

    return {
        .header = header,
        .command_payload = payload,
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


