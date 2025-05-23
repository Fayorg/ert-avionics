// Created by Elie "Fayorg" Baier (elie.baier@epfl.ch) on 23/05/2025.
// Copyright (c) 2025 Elie Baier. All rights reserved.

#ifndef ADDRESSCONFIG_H
#define ADDRESSCONFIG_H

// #define AVIONICS_MAC_ADDRESS {0xEC, 0xE3, 0x34, 0xCE, 0x34, 0x90} // ec:e3:34:ce:34:90
#define AVIONICS_MAC_ADDRESS {0xEC, 0xE3, 0x34, 0xCE, 0x34, 0x90}
#define GROUND_STATION_MAC_ADDRESS {0x20, 0x43,0xA8, 0x64, 0xFF, 0x28}

constexpr uint8_t AVIONICS_MAC_ADDRESS_INT[6] = AVIONICS_MAC_ADDRESS;
constexpr uint8_t GROUND_STATION_MAC_ADDRESS_INT[6] = GROUND_STATION_MAC_ADDRESS;

#endif //ADDRESSCONFIG_H
