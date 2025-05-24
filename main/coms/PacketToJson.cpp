// Created by Elie "Fayorg" Baier (elie.baier@epfl.ch) on 23/05/2025.
// Copyright (c) 2025 Elie Baier. All rights reserved.

#include "PacketToJson.h"

#include "ArduinoJson.h"

std::string PacketToJson::convertTelemetryPacket(esp_now_telemetry_packet_t packet) {
    StaticJsonDocument<sizeof(esp_now_telemetry_payload_t) + sizeof(packet.header.packet_type)> document;
    document["packet_type"] = packet.header.packet_type;

    JsonObject data_obj = document.createNestedObject("payload");
    data_obj["pitch"] = packet.telemetry_payload.pitch;
    data_obj["yaw"] = packet.telemetry_payload.yaw;
    data_obj["roll"] = packet.telemetry_payload.roll;
    data_obj["ax"] = packet.telemetry_payload.accel_x;
    data_obj["ay"] = packet.telemetry_payload.accel_y;
    data_obj["az"] = packet.telemetry_payload.accel_z;
    data_obj["temperature"] = packet.telemetry_payload.temperature_c;
    data_obj["pressure"] = packet.telemetry_payload.pressure_hpa;
    data_obj["altitude"] = packet.telemetry_payload.altitude_m;

    std::string json_output;
    serializeJson(document, json_output);
    return json_output;
}

std::string PacketToJson::convertHeartbeatPacket(esp_now_heartbeat_packet_t packet) {
    StaticJsonDocument<sizeof(esp_now_heartbeat_payload_t) + sizeof(packet.header.packet_type)> document;
    document["packet_type"] = packet.header.packet_type;

    JsonObject data_obj = document.createNestedObject("payload");
    data_obj["status"] = packet.heartbeat_payload.flight_state;

    std::string json_output;
    serializeJson(document, json_output);
    return json_output;
}

