// Created by Elie "Fayorg" Baier (elie.baier@epfl.ch) on 22/05/2025.
// Copyright (c) 2025 Elie Baier. All rights reserved.

#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <cstdint>
#include <cstring>
#include <freertos/projdefs.h>

#include "esp_log.h"
#include "esp_now.h"
#include "coms/PacketConstant.h"

#define WIFI_CHANNEL 6

class Communication {
    public:
        static Communication& getInstance();

        bool init();
        bool init_peer_info(const uint8_t* peer_mac_addr);
        bool register_receive_callback();

        static void on_packet_received(const esp_now_recv_info_t* info, const uint8_t* data, int len);

        uint8_t* get_mac_addr() {
            return mac_addr;
        }

        [[nodiscard]] uint8_t get_sequence_num() const {
            return sequence_num;
        }

        bool send_packet(esp_now_generic_packet_t packet);

        template<typename T>
        bool send_packet(const T& packet, size_t payload_size_bytes) {
            esp_now_generic_packet_t generic_packet;

            generic_packet.header = packet.header;
            generic_packet.crc = packet.crc;

            const uint8_t* specific_payload_ptr = reinterpret_cast<const uint8_t*>(&packet) + sizeof(esp_now_packet_header_t);
            std::memcpy(generic_packet.payload, specific_payload_ptr, payload_size_bytes);

            // Log the size of both packets to compare
            size_t payload_size = payload_size_bytes;
            ESP_LOGI(TAG, "Original payload size: %zu", payload_size);
            ESP_LOGI(TAG, "Generic payload size: %zu", sizeof(generic_packet.payload));

            if (std::memcmp(generic_packet.payload, specific_payload_ptr, payload_size) == 0) {
                ESP_LOGI(TAG, "Payloads are identical");
            } else {
                ESP_LOGW(TAG, "Payloads differ");
            }

            return send_packet(generic_packet);
        }

    private:
        static const char* TAG;
        uint8_t mac_addr[6];
        uint8_t peer[6];
        uint8_t sequence_num = 0;

        Communication() = default;
        Communication(const Communication&) = delete;
        Communication& operator=(const Communication&) = delete;


};

#endif //COMMUNICATION_H
