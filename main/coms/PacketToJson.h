// Created by Elie "Fayorg" Baier (elie.baier@epfl.ch) on 23/05/2025.
// Copyright (c) 2025 Elie Baier. All rights reserved.

#ifndef PACKETTOJSON_H
#define PACKETTOJSON_H
#include <string>

#include "PacketConstant.h"


class PacketToJson {
    public:
        static std::string convertTelemetryPacket(esp_now_telemetry_packet_t packet);
        static std::string convertHeartbeatPacket(esp_now_heartbeat_packet_t packet);
};

#endif //PACKETTOJSON_H
