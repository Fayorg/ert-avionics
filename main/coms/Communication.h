// Created by Elie "Fayorg" Baier (elie.baier@epfl.ch) on 22/05/2025.
// Copyright (c) 2025 Elie Baier. All rights reserved.

#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <cstdint>
#include <freertos/projdefs.h>

#include "coms/PacketConstant.h"

#define WIFI_CHANNEL 6

class Communication {
    public:
        static Communication& getInstance();

        bool init();

        uint8_t* get_mac_addr() {
            return mac_addr;
        }

        [[nodiscard]] uint8_t get_sequence_num() const {
            return sequence_num;
        }

        bool send_packet(esp_now_generic_packet_t packet);

    private:
        static const char* TAG;
        uint8_t mac_addr[6];
        uint8_t sequence_num = 0;

        Communication() = default;
        Communication(const Communication&) = delete;
        Communication& operator=(const Communication&) = delete;


};

#endif //COMMUNICATION_H
