// Created by Elie "Fayorg" Baier (elie.baier@epfl.ch) on 22/05/2025.
// Copyright (c) 2025 Elie Baier. All rights reserved.

#ifndef PACKETCONSTANT_H
#define PACKETCONSTANT_H

#include <cstdint>

#define MAX_ESP_NOW_PAYLOAD_SIZE 250
#define MAX_PACKET_DATA_SIZE (MAX_ESP_NOW_PAYLOAD_SIZE - sizeof(esp_now_packet_header_t) - sizeof(uint16_t)) // Account for header and CRC

using namespace std;

typedef enum {
    PACKET_TYPE_COMMAND = 0x01,
    PACKET_TYPE_ACK = 0x02,
    PACKET_TYPE_TELEMETRY = 0x03,
    PACKET_TYPE_HEARTBEAT = 0x04,
} esp_now_packet_type_t;

// Maybe will work with the CommandRegistry instead
typedef enum {
  CMD_SET_STATE = 0x01,
    CMD_EMA = 0x02,
    CMD_RS = 0x03,
} esp_now_command_id_t;

typedef enum {
    ACK_STATUS_SUCCESS      = 0x01,
    ACK_STATUS_FAILURE      = 0x02,
    ACK_STATUS_INVALID_CMD  = 0x03,
    ACK_STATUS_BUSY         = 0x04, // may not be used idk
} esp_now_ack_status_t;

typedef struct __attribute__((packed)) {
    uint8_t  packet_type;      // esp_now_packet_type_t
    uint8_t  sequence_num;     // Sequence
    uint8_t  sender_mac[6];    // sender's MAC
    uint8_t  reserved;         // Reserved
} esp_now_packet_header_t;

typedef struct __attribute__((packed)) {
    uint8_t  command_id;       // esp_now_command_id_t
    uint8_t  command_data[MAX_PACKET_DATA_SIZE - sizeof(uint8_t)]; // v arible length for the command data
} esp_now_command_payload_t;

typedef struct __attribute__((packed)) {
    esp_now_ack_status_t  ack_status;       // esp_now_ack_status_t
    uint8_t  acked_sequence_num; // Sequence
} esp_now_ack_payload_t;

typedef struct __attribute__((packed)) { // total: 39bytes i thjink which sould be under the 250 smt constraint
    float    temperature_c;
    float    pressure_hpa;
    float    altitude_m;
    float    pitch, roll, yaw;
    float    accel_x, accel_y, accel_z;
    uint8_t  flight_state;

    bool gliding;
    bool parachute_unreefed;
} esp_now_telemetry_payload_t;

typedef struct __attribute__((packed)) {
    uint8_t  flight_state;
} esp_now_heartbeat_payload_t;

// +69+ PACKET DEFINITION +69+ (im to tired for thi shi)
typedef struct __attribute__((packed)) {
    esp_now_packet_header_t header;
    uint8_t                 payload[MAX_PACKET_DATA_SIZE]; // Raw payload data
    uint16_t                crc; // CRC16 checksum of header + payload
} esp_now_generic_packet_t;

typedef struct __attribute__((packed)) {
    esp_now_packet_header_t header;
    esp_now_command_payload_t command_payload;
    uint16_t                crc;
} esp_now_command_packet_t;

typedef struct __attribute__((packed)) {
    esp_now_packet_header_t header;
    esp_now_ack_payload_t   ack_payload;
    uint16_t                crc;
} esp_now_ack_packet_t;

typedef struct __attribute__((packed)) {
    esp_now_packet_header_t header;
    esp_now_telemetry_payload_t telemetry_payload;
    uint16_t                crc;
} esp_now_telemetry_packet_t;

typedef struct __attribute__((packed)) {
    esp_now_packet_header_t header;
    esp_now_heartbeat_payload_t   heartbeat_payload;
    uint16_t                crc;
} esp_now_heartbeat_packet_t;

#endif //PACKETCONSTANT_H
