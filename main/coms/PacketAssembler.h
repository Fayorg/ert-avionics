// Created by Elie "Fayorg" Baier (elie.baier@epfl.ch) on 22/05/2025.
// Copyright (c) 2025 Elie Baier. All rights reserved.

#ifndef PACKETASSEMBLER_H
#define PACKETASSEMBLER_H
#include <PacketConstant.h>

class PacketAssembler {
    public:
        static esp_now_telemetry_packet_t create_telemetry_packet(esp_now_telemetry_payload_t payload);

        static esp_now_heartbeat_packet_t create_heartbeat_packet();

        static esp_now_ack_packet_t create_ack_packet(esp_now_ack_status_t status, uint8_t sequence);

        static esp_now_command_packet_t create_command_packet(esp_now_command_payload_t payload);

    private:
        static esp_now_packet_header_t create_packet_header(uint8_t packet_type);

};

#endif //PACKETASSEMBLER_H
