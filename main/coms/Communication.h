// Created by Elie "Fayorg" Baier (elie.baier@epfl.ch) on 22/05/2025.
// Copyright (c) 2025 Elie Baier. All rights reserved.

#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <cstdint>

#define WIFI_CHANNEL 6

class Communication {
    public:
        static bool init();

        uint8_t* get_mac_addr() {
            return mac_addr;
        }

    private:
        static const char* TAG;
        uint8_t mac_addr[6] = {};


};

#endif //COMMUNICATION_H
