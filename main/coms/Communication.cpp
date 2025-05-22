// Created by Elie "Fayorg" Baier (elie.baier@epfl.ch) on 22/05/2025.
// Copyright (c) 2025 Elie Baier. All rights reserved.

#include "Communication.h"

#include <cstring>
#include <esp_log.h>
#include <esp_now.h>
#include <esp_wifi.h>
#include <nvs_flash.h>
#include <rom/ets_sys.h>

const char* Communication::TAG = "Communication";

Communication& Communication::getInstance() {
    static Communication instance;
    return instance;
}

bool Communication::init() {
    ESP_LOGI(TAG, "Initializing communication...");
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();

    if(nvs_flash_init() != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initialize NVS flash");
        return false;
    }
    if(esp_netif_init() != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initialize netif");
        return false;
    }
    if(esp_event_loop_create_default() != ESP_OK) {
        ESP_LOGE(TAG, "Failed to create event loop");
        return false;
    }

    if(esp_wifi_init(&cfg) != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initialize WiFi");
        return false;
    }
    if(esp_wifi_set_mode(WIFI_MODE_STA) != ESP_OK) {
        ESP_LOGE(TAG, "Failed to set WiFi mode");
        return false;
    }
    if(esp_wifi_start() != ESP_OK) {
        ESP_LOGE(TAG, "Failed to start WiFi");
        return false;
    }
    if(esp_wifi_set_channel(WIFI_CHANNEL, WIFI_SECOND_CHAN_NONE) != ESP_OK) {
        ESP_LOGE(TAG, "Failed to set WiFi channel");
        return false;
    }

    if (esp_wifi_set_protocol(WIFI_IF_STA, WIFI_PROTOCOL_LR) != ESP_OK) {
        ESP_LOGE(TAG, "Failed to set WiFi protocol");
        return false;
    }

    uint8_t mac[6];
    if (esp_wifi_get_mac(WIFI_IF_STA, mac) != ESP_OK) {
        ESP_LOGE(TAG, "Failed to get MAC address");
        return false;
    }
    memcpy(mac_addr, mac, sizeof(mac_addr));

    ESP_LOGI(TAG, "Je ne comprends pas ca ne fait pas le sens... WASSSSSS! Was ist das shit? %02x:%02x:%02x:%02x:%02x:%02x", MAC2STR(mac));
    ESP_LOGI(TAG, "STA MAC: %02x:%02x:%02x:%02x:%02x:%02x", MAC2STR(mac));

    return true;
}

bool Communication::send_packet(esp_now_generic_packet_t packet) {
    if (esp_now_send(mac_addr, reinterpret_cast<uint8_t *>(&packet), sizeof(packet)) != ESP_OK) {
        ESP_LOGW(TAG, "Failed to send packet");
        return false;
    }

    return true;
}

