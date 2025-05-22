//
// Created by Fayorg on 21/05/2025.
//

#include "FlightState.h"

#include <esp_log.h>
#include <TaskRegistry.h>

const char* FlightState::TAG = "FlightState";

FlightState& FlightState::getInstance() {
    static FlightState instance;
    return instance;
}

FlightState::FlightState() : store("flight_state") {
    if (!refreshState()) {
        current_state = INIT;
    }
}

FlightState::State FlightState::getState() const {
    return this->current_state;
}

bool FlightState::setState(State new_state) {
    auto old_state = this->current_state;
    ESP_LOGI(TAG, "Changing state from %d to %d", old_state, new_state);

    if (this->store.setInt("state", new_state) != ESP_OK) {
        ESP_LOGE(TAG, "Failed to set state in NVS");
        return false;
    }
    this->current_state = new_state;

    ESP_LOGI(TAG, "Calling onChangeState callback to TaskRegistry");
    TaskRegistry::getInstance().onStateChange(old_state, new_state);

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

