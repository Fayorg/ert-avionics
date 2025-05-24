// Created by Elie "Fayorg" Baier (elie.baier@epfl.ch) on 22/05/2025.
// Copyright (c) 2025 Elie Baier. All rights reserved.

#include "Communication.h"

#include <cstring>
#include <esp_log.h>
#include <esp_now.h>
#include <esp_wifi.h>
#include <nvs_flash.h>
#include <rom/ets_sys.h>

#include "FlightState.h"
#include "PacketAssembler.h"
#include "PacketToJson.h"
#include "driver/gpio.h"

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

    if (esp_now_init() != ESP_OK) {
        ESP_LOGE(TAG, "Failed to init ESP-NOW");
        return false;
    }

    uint8_t mac[6];
    if (esp_wifi_get_mac(WIFI_IF_STA, mac) != ESP_OK) {
        ESP_LOGE(TAG, "Failed to get MAC address");
        return false;
    }
    memcpy(mac_addr, mac, sizeof(mac_addr));

    ESP_LOGI(TAG, "Je ne comprends pas ca ne fait pas le sens... WASSSSSS! Was ist das shit? %02x:%02x:%02x:%02x:%02x:%02x", MAC2STR(mac));
    ESP_LOGI(TAG, "STA MAC: %02x:%02x:%02x:%02x:%02x:%02x", MAC2STR(mac_addr));

    return true;
}

bool Communication::init_peer_info(const uint8_t *peer_mac_addr) {
    ESP_LOGI(TAG, "Initializing communication peer (%02x:%02x:%02x:%02x:%02x:%02x)", MAC2STR(peer_mac_addr));
    memcpy(peer, peer_mac_addr, sizeof(peer));
    esp_now_peer_info_t peer = {
        .channel = WIFI_CHANNEL,
        .ifidx = WIFI_IF_STA,
        .encrypt = false
    };
    memcpy(peer.peer_addr, peer_mac_addr, ESP_NOW_ETH_ALEN);

    if(esp_now_add_peer(&peer) != ESP_OK) {
        ESP_LOGE(TAG, "Peer init failed");
        return false;
    }
    ESP_LOGI(TAG, "Peer has been established");
    return true;
}

void Communication::on_packet_received(const esp_now_recv_info_t *info, const uint8_t *data, int len) {
    if (info == nullptr || data == nullptr || len == 0) {
        ESP_LOGW(TAG, "Invalid packet received");
        return;
    }

    if (len != sizeof(esp_now_generic_packet_t)) {
        ESP_LOGW(TAG, "Invalid packet size");
        return;
    }

    // Copy the received data into a generic packet
    esp_now_generic_packet_t packet;
    memcpy(&packet, data, sizeof(len));

    // Cast packet to the ciorrec type
    switch (packet.header.packet_type) {
        case PACKET_TYPE_COMMAND:
            ESP_LOGI(TAG, "Received command packet");
            esp_now_command_packet_t command_packet;
            memcpy(&command_packet, data, sizeof(command_packet));

            switch (command_packet.command_payload.command_id) {
                case CMD_SET_STATE: {
                    uint8_t desired_state = command_packet.command_payload.command_data[0];
                    ESP_LOGI(TAG, "Received command to set state: %d", desired_state);

                    if (FlightState::getInstance().setState(static_cast<FlightState::State>(desired_state))) {
                        ESP_LOGI(TAG, "State has been set to %d", desired_state);
                        Communication::getInstance().send_packet(PacketAssembler::create_ack_packet(ACK_STATUS_SUCCESS, command_packet.header.sequence_num), sizeof(esp_now_ack_payload_t));
                    } else {
                        ESP_LOGW(TAG, "Failed to set state to %d", desired_state);
                        Communication::getInstance().send_packet(PacketAssembler::create_ack_packet(ACK_STATUS_FAILURE, command_packet.header.sequence_num), sizeof(esp_now_ack_payload_t));
                    }
                    break;
                }

                case CMD_EMA: {
                    ESP_LOGI(TAG, "Received command to emergency EMATCH");
                    gpio_set_level(GPIO_NUM_4, 1);
                    vTaskDelay(pdMS_TO_TICKS(5000));
                    gpio_set_level(GPIO_NUM_4, 0);
                }

                case CMD_RS: {
                    uint8_t desired_state = command_packet.command_payload.command_data[0];
                    if (desired_state == 1) {
                        // TODO: reset storage
                    } else {
                        ESP_LOGI(TAG, "Received command to reset state");
                        esp_restart();
                    }
                }
                default:
                    ESP_LOGW(TAG, "Unknown command ID");
                    Communication::getInstance().send_packet(PacketAssembler::create_ack_packet(ACK_STATUS_INVALID_CMD, command_packet.header.sequence_num), sizeof(esp_now_ack_payload_t));
                    break;
            }

            break;
        case PACKET_TYPE_ACK:
            ESP_LOGI(TAG, "Received ACK packet");
            break;
        case PACKET_TYPE_HEARTBEAT: {
            ESP_LOGI(TAG, "Received heartbeat packet");

            esp_now_heartbeat_packet_t heartbeat_packet;
            memcpy(&heartbeat_packet, data, sizeof(heartbeat_packet));

            auto json = PacketToJson::convertHeartbeatPacket(heartbeat_packet);
            printf("*%s\n", json.c_str());
            break;
        }
        case PACKET_TYPE_TELEMETRY: {
            ESP_LOGI(TAG, "Received telemetry packet");

            esp_now_telemetry_packet_t telemetry_packet;
            memcpy(&telemetry_packet, data, sizeof(telemetry_packet));

            auto json = PacketToJson::convertTelemetryPacket(telemetry_packet);
            printf("*%s\n", json.c_str());
            break;
        }
        default:
            ESP_LOGW(TAG, "Unknown packet type");
            break;
    }
}

bool Communication::register_receive_callback() {
    if (esp_now_register_recv_cb(on_packet_received) != ESP_OK) {
        ESP_LOGE(TAG, "Failed to register receive callback");
        return false;
    }
    ESP_LOGI(TAG, "Receive callback registered");
    return true;
}



bool Communication::send_packet(esp_now_generic_packet_t packet) {
    if (auto err = esp_now_send(peer, reinterpret_cast<uint8_t *>(&packet), sizeof(packet)) != ESP_OK) {
        ESP_LOGW(TAG, "Failed to send packet (%d)", err);
        return false;
    }

    return true;
}