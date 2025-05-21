#include <esp_log.h>
#include <FlightState.h>
#include <NVSStore.h>
#include <SpiffsStorage.h>
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

    // Testing the SPIFFS Storage
    SpiffsStorage spiffsStorage;
    if (!spiffsStorage.begin()) {
        ESP_LOGE("SPIFFS", "Failed to initialize SPIFFS");
        return;
    }

    spiffsStorage.writeFile("/test.txt", "Hello SPIFFS!");
    ESP_LOGI("MAIN-SPIFFS", "Read from SPIFFS: %s", spiffsStorage.readFile("/test.txt").c_str());

    spiffsStorage.end();
}
