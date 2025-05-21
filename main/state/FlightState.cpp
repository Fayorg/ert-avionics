//
// Created by Fayorg on 21/05/2025.
//

#include "FlightState.h"

#include <esp_log.h>

const char* FlightState::TAG = "FlightState";

FlightState::FlightState(): store("flight_state") {
    // Get state from NVS otherwise set to INIT
    if (!this->refreshState()) {
        this->current_state = INIT;
    }
}

FlightState::State FlightState::getState() const {
    return this->current_state;
}

bool FlightState::setState(State new_state) {
    ESP_LOGI("Changing state to ", "%d", new_state);

    if (this->store.setInt("state", new_state) != ESP_OK) {
        ESP_LOGE(TAG, "Failed to set state in NVS");
        return false;
    }
    this->current_state = new_state;

    return true;
}


bool FlightState::refreshState() {
    const auto state = this->store.getInt("state", -1);
    if (state == -1) {
        ESP_LOGE(TAG, "Failed to get state from NVS");
        return false;
    }

    this->current_state = static_cast<State>(state);
    return true;
}

