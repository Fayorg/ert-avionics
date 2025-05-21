#include <esp_log.h>
#include <FlightState.h>
#include <NVSStore.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

static FlightState* flightState = nullptr;

extern "C" void app_main(void)
{
    // Init
    if (NVSStore::initNVSFlash() != ESP_OK) {
        ESP_LOGE("Avionics-Init", "Failed to initialize NVS flash. Aborting.");
        while(1) { vTaskDelay(pdMS_TO_TICKS(1000)); }
    }
    flightState = new FlightState();

    vTaskDelay(pdMS_TO_TICKS(1000));

    flightState->setState(FlightState::READY);
    ESP_LOGI("Flight state", "Current state: %d", flightState->getState());

    vTaskDelay(pdMS_TO_TICKS(2000));
    flightState->setState(FlightState::LANDED);
}
