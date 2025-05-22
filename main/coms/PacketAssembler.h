// Created by Elie "Fayorg" Baier (elie.baier@epfl.ch) on 22/05/2025.
// Copyright (c) 2025 Elie Baier. All rights reserved.

#ifndef PACKETASSEMBLER_H
#define PACKETASSEMBLER_H
#include <PacketConstant.h>

class PacketAssembler {
    public:
        static esp_now_telemetry_packet_t create_telemetry_packet();

        static esp_now_heartbeat_packet_t create_heartbeat_packet();

    private:
        static esp_now_packet_header_t create_packet_header(uint8_t packet_type);

};

#endif //PACKETASSEMBLER_H
